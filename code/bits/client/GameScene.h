#ifndef BITS_CLIENT_GAME_SCENE_H
#define BITS_CLIENT_GAME_SCENE_H

#include <gf/Scene.h>

#include "ArmySelection.h"
#include "GameState.h"
#include "HUD.h"
#include "MapRender.h"
#include "RegimentRender.h"

namespace gw {
  class GameScene: public gf::Scene {
  public:
    GameScene(const gf::Vector2i &initializeSize, GameState &gameState);

  private:
    void doProcessEvent(gf::Event &event) override;
    void doUpdate(gf::Time time) override;

    void processPackets();

  private:
    GameState& m_gameState;

    // gf::ZoomingViewAdaptor m_adaptor; // TODO: BROKEN

    // Main entities
    MapRender m_mapRender;
    RegimentRender m_regimentsRender;

    // HUD entities
    HUD m_hud;

    // State machines
    // ArmySelection m_armySelection; // TODO: BROKEN
  };
}

#endif // BITS_CLIENT_GAME_SCENE_H
