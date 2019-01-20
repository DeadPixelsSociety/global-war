#ifndef BITS_SERVER_THREAD_COM_H
#define BITS_SERVER_THREAD_COM_H

#include <gf/Id.h>
#include <gf/Queue.h>

#include "../common/Packet.h"
#include "../common/Sockets.h"

namespace gw {
  class ThreadCom {
  public:
    ThreadCom(SocketTcp socket, gf::Queue<Packet> &queue);

    void setQueue(gf::Queue<Packet>* queue);

    void start();
    void sendPacket(Packet &packet);

  private:
    void receivePackets();

  private:
    SocketTcp m_socket;
    gf::Queue<Packet>* m_queue;
  };
}

#endif // BITS_SERVER_THREAD_COM_H
