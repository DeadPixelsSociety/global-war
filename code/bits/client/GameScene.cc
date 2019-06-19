#include "GameScene.h"

#include <gf/Log.h>
#include <gf/Unused.h>

#include "Singletons.h"

namespace gw {
  GameScene::GameScene(const gf::Vector2i &initializeSize, GameState &gameState)
  : Scene(initializeSize)
  , m_gameState(gameState)
  // , m_adaptor(m_renderer, m_mainView)
  , m_mapRender(gameState)
  , m_regimentsRender(gameState)
  , m_hud(gameState)
  // , m_armySelection(gameState)
  {
    addMainEntity(m_mapRender);
    addMainEntity(m_regimentsRender);

    addHudEntity(m_hud);

    // Register for a quick match
    m_gameState.quickMatch();

    // Send the acknowledge to join session game
    Packet packet;
    packet.type = PacketType::AckJoinGame;
    bool ok = m_gameState.threadCom.sendPacket(packet);
    assert(ok);
  }

  void GameScene::doProcessEvent(gf::Event &event) {
    // TODO: BROKEN
    // m_adaptor.processEvent(event);
    // m_armySelection.processEvent(event);
  }

  void GameScene::doUpdate(gf::Time time) {
    gf::unused(time);
    m_gameState.update(time);

    processPackets();
  }

  void GameScene::processPackets() {
    Packet packet;
    while (m_gameState.comQueue.poll(packet)) {
      switch (packet.type) {
        case PacketType::NewPlayer:
        case PacketType::QuickMatch:
        case PacketType::MoveRegiment:
        case PacketType::JoinGame:
        case PacketType::AckJoinGame:
          gf::Log::error("Receive unexpected packet in game Scene\n");
          assert(false);
          break;

        case PacketType::CreateRegiment:
          gf::Log::info("Regiment created: {count: %d, pos: %d,%d, owner: %lx}\n",
            packet.createRegiment.count, packet.createRegiment.position.x,
            packet.createRegiment.position.y, packet.createRegiment.ownerID);

          m_gameState.data.regiments.push_back({packet.createRegiment.ownerID, packet.createRegiment.count, packet.createRegiment.position, packet.createRegiment.division});
          break;

        case PacketType::MoveUnit:
          gf::Log::info("Move unit from {%d,%d} to {%d,%d}\n",
            packet.moveUnit.origin.x, packet.moveUnit.origin.y,
            packet.moveUnit.destination.x, packet.moveUnit.destination.y);

          m_gameState.data.moveUnit(packet.moveUnit.origin, packet.moveUnit.destination);
          break;

        case PacketType::KillUnit:
          gf::Log::info("Kill unit at {%d,%d}\n",
            packet.killUnit.position.x, packet.killUnit.position.y);

          m_gameState.data.killUnit(packet.killUnit.position);

          break;

        case PacketType::InitializePlayer:
          assert(packet.initializePlayer.playerID == m_gameState.currentPlayerID);

          gf::Log::info("Inital data: {playerID: %lx, pos: %d,%d}\n", packet.initializePlayer.playerID, packet.initializePlayer.position.x, packet.initializePlayer.position.y);
          // TODO: BROKEN
          // m_mainView.setCenter(Hexagon::positionToCoordinates(packet.initializePlayer.position));

          break;

        case PacketType::WinGame:
          gf::Log::info("Winner playerID: %lx\n", packet.winGame.winner);

          break;
      }
    }
  }
}
