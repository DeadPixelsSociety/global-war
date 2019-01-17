#include "PlayerCom.h"

#include <iostream>
#include <thread>

#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

namespace gw {

  PlayerCom::PlayerCom(SocketTcp socket, gf::Queue<Packet> &queue, gf::Id playerId)
  : m_socket(std::move(socket))
  , m_queue(&queue)
  , m_playerId(playerId) {

  }

  void PlayerCom::start() {
    std::thread(&PlayerCom::receivePackets, this).detach();
  }

  void PlayerCom::receivePackets() {
    for(;;) {
      Packet packet;
      m_socket.receive(packet);

      if (m_socket.getState() == SocketState::Disconnected) {
        break;
      }

      m_queue->push(packet);
    }
  }

  void PlayerCom::sendPacket(Packet &packet) {
    m_socket.send(packet);
  }

}
