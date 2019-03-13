#include "LobbyStage.h"

#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Log.h>
#include <gf/Event.h>

#include "Singletons.h"

namespace gw {
  LobbyStage::LobbyStage(gf::Window &window, gf::RenderWindow &renderer, GameState &gameState)
  : Stage(window, renderer, gameState)
  , m_waitScreen(gResourceManager().getFont("DejaVuSans.ttf"))
  , m_gameFound(false) {
    m_views.addView(m_hudView);
    m_views.setInitialScreenSize(InitialScreenSize);

    m_entities.addEntity(m_waitScreen);
  }

  void LobbyStage::loop() {
    // Register for a quick match
    m_gameState.quickMacth();
    m_gameFound = false;

    m_renderer.clear(gf::Color::White);

    gf::Clock clock;

    while (m_window.isOpen() && !m_gameFound) {
      gf::Event event;

      while (m_window.pollEvent(event)) {
        m_actions.processEvent(event);
        m_views.processEvent(event);
      }

      if (m_closeWindowAction.isActive()) {
        m_window.close();
      }

      if (m_fullscreenAction.isActive()) {
        m_window.toggleFullscreen();
      }

      gf::Time time = clock.restart();
      m_entities.update(time);

      m_renderer.clear();

      m_renderer.setView(m_hudView);
      m_entities.render(m_renderer);

      m_renderer.display();

      m_actions.reset();

      // Process all pending packets
      processPackets();
    }
  }

  void LobbyStage::processPackets() {
    Packet packet;
    while (m_gameState.comQueue.poll(packet)) {
      switch (packet.type) {
        case PacketType::Ping:
        case PacketType::NewPlayer:
        case PacketType::QuickMatch:
        case PacketType::CreateRegiment:
        case PacketType::MoveRegiment:
          gf::Log::error("Receive unexpected packet in lobby\n");
          break;

        case PacketType::JoinGame:
          gf::Log::info("Game ID: %lx\n", packet.joinGame.gameID);
          gf::Log::info("Number players: %d\n", packet.joinGame.nbPlayers);
          for (int i = 0; i < packet.joinGame.nbPlayers; ++i) {
            gf::Log::info("Players[%d] ID = %lx\n", i, packet.joinGame.playersID[i]);
          }

          // Add playerIDs to the client model
          auto it = packet.joinGame.playersID.begin();
          m_gameState.allPlayerID.insert(m_gameState.allPlayerID.begin(), it, it+packet.joinGame.nbPlayers);

          m_gameFound = true;
          break;
      }
    }

  }
}
