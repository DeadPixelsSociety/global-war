#include "GameState.h"

#include <gf/Color.h>
#include <gf/Vector.h>

namespace gw {
  static constexpr gf::Vector2u ScreenSize(1024, 576);
  static constexpr gf::Vector2f ViewSize(1000.0f, 1000.0f); // dummy values
  static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f); // dummy values

  GameState::GameState(char* hostname, char *port, gf::ResourceManager &resources)
  : m_state(State::Lobby)
  , m_window("Global War", ScreenSize)
  , m_renderer(m_window)
  , m_mainView(ViewCenter, ViewSize)
  , m_adaptor(m_renderer, m_mainView)
  , m_closeWindowAction("Close window")
  , m_fullscreenAction("Fullscreen")
  , m_leftAction("Left")
  , m_rightAction("Right")
  , m_upAction("Up")
  , m_downAction("Down")
  , m_pingAction("Ping")
  , m_waitScreen(resources.getFont("DejaVuSans.ttf"))
  , m_map(resources.getAbsolutePath("map.txt"))
  , m_army(resources)
  , m_armySelection(m_map, m_army, m_renderer, m_mainView) {
    // Init screen
    m_window.setVerticalSyncEnabled(true);
    m_window.setFramerateLimit(60);

    // Init network
    m_socket.connectTo(hostname, port);

    // Receive ID
    auto packet = newPlayer();
    m_playerID = packet.newPlayer.playerID;
    gf::Log::info("Player ID: %lx\n", m_playerID);

    // Init views
    m_lobbyViews.addView(m_hudView);
    m_lobbyViews.setInitialScreenSize(ScreenSize);

    m_gameViews.addView(m_mainView);
    m_gameViews.addView(m_hudView);
    m_gameViews.setInitialScreenSize(ScreenSize);

    // Init actions
    m_closeWindowAction.addCloseControl();
    m_closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
    m_lobbyActions.addAction(m_closeWindowAction);
    m_gameActions.addAction(m_closeWindowAction);

    m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    m_lobbyActions.addAction(m_fullscreenAction);
    m_gameActions.addAction(m_fullscreenAction);

    m_leftAction.addScancodeKeyControl(gf::Scancode::A);
    m_leftAction.addScancodeKeyControl(gf::Scancode::Left);
    m_leftAction.setContinuous();
    m_gameActions.addAction(m_leftAction);

    m_rightAction.addScancodeKeyControl(gf::Scancode::D);
    m_rightAction.addScancodeKeyControl(gf::Scancode::Right);
    m_rightAction.setContinuous();
    m_gameActions.addAction(m_rightAction);

    m_upAction.addScancodeKeyControl(gf::Scancode::W);
    m_upAction.addScancodeKeyControl(gf::Scancode::Up);
    m_upAction.setContinuous();
    m_gameActions.addAction(m_upAction);

    m_downAction.addScancodeKeyControl(gf::Scancode::S);
    m_downAction.addScancodeKeyControl(gf::Scancode::Down);
    m_downAction.setContinuous();
    m_gameActions.addAction(m_downAction);

    m_pingAction.addScancodeKeyControl(gf::Scancode::P);
    m_gameActions.addAction(m_pingAction);

    // Init entities
    m_lobbyEntities.addEntity(m_waitScreen);

    m_gameEntities.addEntity(m_map);
    m_gameEntities.addEntity(m_army);
  }

  void GameState::loop() {
    switch (m_state) {
      case State::Lobby:
        lobbyLoop();
        break;

      case State::Game:
        gameLoop();
        break;
    }
  }

  void GameState::ping() {
    gw::Packet packet;
    packet.type = PacketType::Ping;
    packet.ping.sequence = 42;

    m_socket.send(packet);
  }

  void GameState::quickMacth() {
    Packet packet;
    packet.type = PacketType::QuickMatch;
    packet.quickMatch.playerID = m_playerID;

    m_socket.send(packet);
  }

  Packet GameState::newPlayer() {
    Packet packet;
    m_socket.receive(packet);

    assert(packet.type == PacketType::NewPlayer);

    return packet;
  }

  void GameState::lobbyLoop() {
    // register for a quick match
    quickMacth();

    m_renderer.clear(gf::Color::White);

    gf::Clock clock;

    while (m_window.isOpen()) {
      gf::Event event;

      while (m_window.pollEvent(event)) {
        m_lobbyActions.processEvent(event);
        m_lobbyViews.processEvent(event);
      }

      if (m_closeWindowAction.isActive()) {
        m_window.close();
      }

      if (m_fullscreenAction.isActive()) {
        m_window.toggleFullscreen();
      }

      if (m_pingAction.isActive()) {
        ping();
      }

      gf::Time time = clock.restart();
      m_lobbyEntities.update(time);

      m_renderer.clear();

      m_renderer.setView(m_hudView);
      m_lobbyEntities.render(m_renderer);

      m_renderer.display();

      m_lobbyActions.reset();
    }
  }

  void GameState::gameLoop() {
    m_renderer.clear(gf::Color::White);

    gf::Clock clock;

    while (m_window.isOpen()) {
      // 1. input

      gf::Event event;

      while (m_window.pollEvent(event)) {
        m_gameActions.processEvent(event);
        m_gameViews.processEvent(event);
        m_adaptor.processEvent(event);
        m_armySelection.processEvent(event);
      }

      if (m_closeWindowAction.isActive()) {
        m_window.close();
      }

      if (m_fullscreenAction.isActive()) {
        m_window.toggleFullscreen();
      }

      if (m_rightAction.isActive()) {
        // do something
      } else if (m_leftAction.isActive()) {
        // do something
      } else if (m_upAction.isActive()) {
        // do something
      } else if (m_downAction.isActive()) {
        // do something
      } else {
        // do something
      }

      if (m_pingAction.isActive()) {
        ping();
      }

      // 2. update

      gf::Time time = clock.restart();
      m_gameEntities.update(time);
      m_hudEntities.update(time);


      // 3. draw

      m_renderer.clear();

      m_renderer.setView(m_mainView);
      m_gameEntities.render(m_renderer);

      m_renderer.setView(m_hudView);
      m_hudEntities.render(m_renderer);

      m_renderer.display();

      m_gameActions.reset();
    }
  }
}
