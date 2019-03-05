#ifndef BITS_CLIENT_GAME_STAGE_H
#define BITS_CLIENT_GAME_STAGE_H

#include <gf/EntityContainer.h>
#include <gf/ResourceManager.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>

#include "ArmySelection.h"
#include "ClientArmy.h"
#include "ClientMap.h"
#include "ClientModel.h"
#include "HUD.h"
#include "Stage.h"

namespace gw {
  class GameStage : public Stage {
  public:
    GameStage(gf::Window &window, gf::RenderWindow &renderer, ClientModel &clientModel, gf::ResourceManager &resources);

    void loop() override;
    void processPackets() override;

  private:
    gf::ExtendView m_mainView;
    gf::ScreenView m_hudView;
    gf::ZoomingViewAdaptor m_adaptor;

    gf::EntityContainer m_mainEntities;
    ClientMap m_map;
    ClientArmy m_army;
    gf::EntityContainer m_hudEntities;
    HUD m_hud;

    // State machines
    ArmySelection m_armySelection;
  };
}

#endif // BITS_CLIENT_GAME_STAGE_H
