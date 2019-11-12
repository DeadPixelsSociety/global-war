#ifndef BITS_CLIENT_LOBBY_SCENE_H
#define BITS_CLIENT_LOBBY_SCENE_H

#include <gf/Scene.h>

#include "GameState.h"
#include "NetworkManagerClient.h"
#include "WaitScreen.h"

namespace gw {
  class LobbyScene: public gf::Scene {
  public:
    LobbyScene(const gf::Vector2i &initializeSize, GameState &gameState);

  private:
    void doUpdate(gf::Time time) override;

    void processPackets();

  private:
    GameState& m_gameState;
    WaitScreen m_waitScreen;
  };
}

#endif // BITS_CLIENT_LOBBY_SCENE_H
