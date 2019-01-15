#include "PlayerCom.h"

#include <iostream>
#include <thread>

#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

using boost::asio::ip::tcp;

namespace gw {

  PlayerCom::PlayerCom(tcp::socket sock, gf::Queue<Packet> &queue, gf::Id playerId)
  : m_sock(std::move(sock))
  , m_queue(&queue)
  , m_playerId(playerId) {
    std::thread(&PlayerCom::receivePackets, this).detach();
  }

  void PlayerCom::receivePackets() {
    constexpr int MaxLength = 1024;

    for (;;) {
      uint8_t data[MaxLength];

      boost::system::error_code error;

      size_t length = m_sock.read_some(boost::asio::buffer(data), error);

      if (error == boost::asio::error::eof) {
        break; // Connection closed cleanly by peer.
      } else if (error) {
        throw boost::system::system_error(error); // Some other error.
      }

      gf::MemoryInputStream stream(gf::ArrayRef<uint8_t>(data, length));
      gf::Deserializer ar(stream);

      Packet packet;
      ar | packet;

      m_queue->push(packet);
    }
  }

  // Thread safe ? I don't think @ahugeat
  void PlayerCom::sendPacket(Packet &packet) {
    constexpr int MaxLength = 1024;
    uint8_t data[MaxLength];

    gf::MemoryOutputStream stream(data);
    gf::Serializer ar(stream);

    ar | packet;

    boost::asio::write(m_sock, boost::asio::buffer(data, MaxLength));
  }

}
