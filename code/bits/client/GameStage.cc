#include "GameStage.h"

#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Log.h>
#include <gf/Event.h>

namespace gw {
  GameStage::GameStage(gf::Window &window, gf::RenderWindow &renderer, ClientModel &clientModel, gf::ResourceManager &resources)
  : Stage(window, renderer, clientModel)
  , m_adaptor(m_renderer, m_mainView)
  , m_map(resources.getAbsolutePath("map.txt"))
  , m_army(resources, clientModel)
  , m_hud(clientModel)
  , m_armySelection(m_map, m_army, clientModel, m_renderer, m_mainView) {
    m_views.addView(m_mainView);
    m_views.addView(m_hudView);
    m_views.setInitialScreenSize(InitialScreenSize);

    // m_entities.addEntity(m_waitScreen);
    m_mainEntities.addEntity(m_map);
    m_mainEntities.addEntity(m_army);

    m_hudEntities.addEntity(m_hud);
  }

  void GameStage::loop() {
    m_renderer.clear(gf::Color::White);

    gf::Clock clock;

    while (m_window.isOpen()) {
      gf::Event event;

      while (m_window.pollEvent(event)) {
        m_actions.processEvent(event);
        m_views.processEvent(event);
        m_adaptor.processEvent(event);
        m_armySelection.processEvent(event);
      }

      if (m_closeWindowAction.isActive()) {
        m_window.close();
      }

      if (m_fullscreenAction.isActive()) {
        m_window.toggleFullscreen();
      }

      gf::Time time = clock.restart();
      m_mainEntities.update(time);
      m_hudEntities.update(time);


      m_renderer.clear();

      m_renderer.setView(m_mainView);
      m_mainEntities.render(m_renderer);

      m_renderer.setView(m_hudView);
      m_hudEntities.render(m_renderer);

      m_renderer.display();

      m_actions.reset();

      // Process all pending packets
      processPackets();
    }
  }

  void GameStage::processPackets() {
    Packet packet;
    while (m_clientModel.comQueue.poll(packet)) {
      switch (packet.type) {
        case PacketType::Ping:
        case PacketType::NewPlayer:
        case PacketType::QuickMatch:
        case PacketType::MoveRegiment:
        case PacketType::JoinGame:
          gf::Log::error("Receive unexpected packet in lobby\n");
          break;

        case PacketType::CreateRegiment:
          gf::Log::info("Regiment created: {count: %d, pos: %d,%d, owner: %lx}\n",
            packet.createRegiment.count, packet.createRegiment.position.x,
            packet.createRegiment.position.y, packet.createRegiment.ownerID);

          m_army.createRegiment(packet.createRegiment.count, packet.createRegiment.position, packet.createRegiment.ownerID);
          break;
      }
    }

  }
}
