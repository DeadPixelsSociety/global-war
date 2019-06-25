#ifndef BITS_CLIENT_GAME_SCENE_H
#define BITS_CLIENT_GAME_SCENE_H

#include <gf/RenderTarget.h>
#include <gf/Scene.h>

#include "ArmySelection.h"
#include "GameState.h"
#include "HUD.h"
#include "MapRender.h"
#include "RegimentRender.h"

namespace gw {
  class GameScene: public gf::Scene {
  public:
    GameScene(const gf::Vector2i &initializeSize, GameState &gameState, const gf::RenderTarget& renderer);

  private:
    void doProcessEvent(gf::Event &event) override;
    void doUpdate(gf::Time time) override;

    void processPackets();

  private:
    GameState& m_gameState;

    gf::ZoomingViewAdaptor m_adaptor;

    // Main entities
    MapRender m_mapRender;
    RegimentRender m_regimentsRender;

    // HUD entities
    HUD m_hud;

    // State machines
    ArmySelection m_armySelection;
  };
}

#endif // BITS_CLIENT_GAME_SCENE_H
