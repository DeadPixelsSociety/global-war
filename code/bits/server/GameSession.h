#ifndef GW_SERVER_MATCH
#define GW_SERVER_MATCH

#include <vector>

#include <gf/Log.h>
#include <gf/Queue.h>

#include "../common/Packet.h"

#include "GameState.h"
#include "Player.h"

namespace gw {
  class GameSession {
  public:
    GameSession(gf::Id gameID, std::map<gf::Id, Player> &players);

    void launchGame();

    void sendAtPlayers(Packet &packet);

  private:
    gf::Id m_gameID;
    gf::Queue<Packet> m_queue;
    std::vector<Player*> m_players;
    GameState m_gameState;
  };
}


#endif // GW_SERVER_MATCH
