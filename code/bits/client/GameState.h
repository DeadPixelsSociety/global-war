#ifndef BITS_CLIENT_GAME_STATE_H
#define BITS_CLIENT_GAME_STATE_H

#include <gf/Queue.h>

#include "../common/Data.h"
#include "../common/ThreadCom.h"

namespace gw {
  static constexpr gf::Vector2u InitialScreenSize(1024, 576);

  struct GameState {
    GameState(char *hostname, char* port);

    // Network
    gf::Queue<Packet> comQueue;
    ThreadCom threadCom;
    void quickMacth();

    // Players management
    gf::Id currentPlayerID;
    std::vector<gf::Id> allPlayerID;

    gf::Color4f getPlayerColor();
    gf::Color4f getPlayerColor(gf::Id playerID);

    // Game data
    Data data;
  };

}

#endif // BITS_CLIENT_GAME_STATE_H
