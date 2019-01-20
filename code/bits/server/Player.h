#ifndef BITS_SERVER_PLAYER_H
#define BITS_SERVER_PLAYER_H

#include "../common/ThreadCom.h"

namespace gw {

  class Player {
  public:
    enum class State {
      Idle,
      WaitingMatch,
      Playing,
    };

  public:
    Player(SocketTcp socket, gf::Queue<Packet> &queue, gf::Id playerID);

    State getState() const;

    void waitGame();
    void plays();

    void sendPacket(Packet &packet);
    void initialize();
    void updateQueue(gf::Queue<Packet> *queue);

  private:
    ThreadCom m_com;
    State m_state;
    gf::Id m_playerID;
  };

}

#endif
