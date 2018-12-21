#include "PlayerCom.h"

#include <iostream>
#include <thread>

#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

#include "../common/Packet.h"

using boost::asio::ip::tcp;

namespace gw {

  PlayerCom::PlayerCom(tcp::socket sock)
  : m_sock(std::move(sock)) {
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
        std::cout << "Thread error" << std::endl;
        throw boost::system::system_error(error); // Some other error.
      }

      gf::MemoryInputStream stream(gf::ArrayRef<uint8_t>(data, length));
      gf::Deserializer ar(stream);

      Packet packet;
      ar | packet;

      assert(packet.type == gw::PacketType::Ping);
      std::cout << "Length: " << length << '\n';
      std::cout << "Ping: " << packet.ping.sequence << '\n';

    }
  }

}
