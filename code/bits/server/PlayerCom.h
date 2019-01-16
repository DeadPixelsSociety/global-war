#ifndef BITS_SERVER_PLAYER_COM
#define BITS_SERVER_PLAYER_COM

#include <gf/Id.h>
#include <gf/Queue.h>

#include "../common/Packet.h"
#include "../common/Sockets.h"

namespace gw {
  class PlayerCom {
  public:
    PlayerCom(SocketTcp socket, gf::Queue<Packet> &queue, gf::Id playerId);

    void start();
    void sendPacket(Packet &packet);

  private:
    void receivePackets();

  private:
    SocketTcp m_socket;
    gf::Queue<Packet>* m_queue;
    gf::Id m_playerId;
  };
}

#endif // BITS_SERVER_PLAYER_COM
