#ifndef BITS_SERVER_THREAD_COM_H
#define BITS_SERVER_THREAD_COM_H

#include <functional>

#include <gf/Id.h>
#include <gf/Queue.h>

#include "Packet.h"
#include "Sockets.h"

namespace gw {
  class ThreadCom {
  public:
    ThreadCom(SocketTcp socket, gf::Queue<Packet> &queue);
    ThreadCom(char* hostname, char *port, gf::Queue<Packet> &queue);

    void setQueue(gf::Queue<Packet>* queue);

    void start();
    bool sendPacket(Packet &packet);
    bool receivePacket(Packet &packet);
    void receivePackets();

  private:
    SocketTcp m_socket;
    gf::Queue<Packet>* m_queue;
  };
}

#endif // BITS_SERVER_THREAD_COM_H
