#ifndef GW_SERVER_MATCH
#define GW_SERVER_MATCH

#include <vector>

#include <gf/Log.h>
#include <gf/Queue.h>

#include "GameState.h"
#include "NetworkManagerServer.h"
#include "Singletons.h"

namespace gw {
  class GameSession {
  public:
    GameSession(NetworkManagerServer& network, gf::Id gameID, std::vector<gf::Id> players);

    void ackPlayer();

    void launchGame();

    // void sendAtPlayers(Packet &packet);

  private:
    void initializeGame();

  private:
    NetworkManagerServer& m_network;
    gf::Id m_gameID;
    std::vector<gf::Id> m_players;
    int m_waitingAck;
    GameState m_gameState;
  };
}


#endif // GW_SERVER_MATCH
