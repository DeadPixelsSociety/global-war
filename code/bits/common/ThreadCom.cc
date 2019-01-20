#include "ThreadCom.h"

#include <iostream>
#include <thread>

#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

namespace gw {

  ThreadCom::ThreadCom(SocketTcp socket, gf::Queue<Packet> &queue)
  : m_socket(std::move(socket))
  , m_queue(&queue) {

  }

  ThreadCom::ThreadCom(char* hostname, char *port, gf::Queue<Packet> &queue)
  : m_queue(&queue) {
    m_socket.connectTo(hostname, port);
  }

  void ThreadCom::setQueue(gf::Queue<Packet>* queue) {
    m_queue = queue;
  }

  void ThreadCom::start() {
    std::thread(&ThreadCom::receivePackets, this).detach();
  }

  void ThreadCom::sendPacket(Packet &packet) {
    m_socket.send(packet);
  }

  Packet ThreadCom::receivePacket() {
    Packet packet;
    m_socket.receive(packet);

    return packet;
  }

  void ThreadCom::receivePackets() {
    for(;;) {
      Packet packet;
      m_socket.receive(packet);

      if (m_socket.getState() == SocketState::Disconnected) {
        break;
      }

      m_queue->push(packet);
    }
  }

}