#ifndef BITS_CLIENT_GAME_STATE_H
#define BITS_CLIENT_GAME_STATE_H

#include <gf/Action.h>
#include <gf/Entity.h>
#include <gf/EntityContainer.h>
#include <gf/RenderWindow.h>
#include <gf/ResourceManager.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "../common/Packet.h"
#include "../common/Sockets.h"
#include "../common/ThreadCom.h"

#include "ArmySelection.h"
#include "ClientArmy.h"
#include "ClientMap.h"
#include "ClientModel.h"
#include "HUD.h"
#include "WaitScreen.h"

namespace gw {
  class GameState: public gf::Entity {
  public:
    GameState(char* hostname, char *port, gf::ResourceManager &resources);

    void loop();

  private:
    void ping();
    void quickMacth();
    void newPlayer();

    void lobbyLoop();
    void lobbyProcessPackets();

    void gameLoop();
    void gameProcessPackets();


  private:
    enum class State {
      Lobby,
      Game,
    };

  private:
    // State
    State m_state;

    // Screen
    gf::Window m_window;
    gf::RenderWindow m_renderer;

    // Network
    gf::Queue<Packet> m_comQueue;
    ThreadCom m_threadCom;

    // Views
    gf::ExtendView m_mainView;

    // for game
    gf::ViewContainer m_lobbyViews;

    // for game
    gf::ViewContainer m_gameViews;
    gf::ScreenView m_hudView;
    gf::ZoomingViewAdaptor m_adaptor;

    // Actions
    gf::ActionContainer m_lobbyActions;
    gf::ActionContainer m_gameActions;
    gf::Action m_closeWindowAction;
    gf::Action m_fullscreenAction;
    gf::Action m_leftAction;
    gf::Action m_rightAction;
    gf::Action m_upAction;
    gf::Action m_downAction;
    gf::Action m_pingAction;

    // Model
    ClientModel m_clientModel;

    // Entities
    // for lobby
    gf::EntityContainer m_lobbyEntities;
    WaitScreen m_waitScreen;

    // for game
    gf::EntityContainer m_gameEntities;
    ClientMap m_map;
    ClientArmy m_army;
    gf::EntityContainer m_hudEntities;
    HUD m_hud;

    // State machines
    ArmySelection m_armySelection;
  };
}

#endif // BITS_CLIENT_GAME_STATE_H
