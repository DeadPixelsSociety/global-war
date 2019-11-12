#include "GameScene.h"

#include <gf/Log.h>
#include <gf/Unused.h>

#include "../common/Hexagon.h"

#include "Singletons.h"

namespace gw {
  GameScene::GameScene(const gf::Vector2i &initializeSize, GameState &gameState, const gf::RenderTarget& renderer)
  : Scene(initializeSize)
  , m_gameState(gameState)
  , m_adaptor(renderer, getWorldView())
  , m_mapRender(gameState)
  , m_regimentsRender(gameState)
  , m_hud(gameState)
  , m_armySelection(gameState, renderer, getWorldView())
  {
    setWorldViewSize({Hexagon::Size * 10, Hexagon::Size * 10});

    addWorldEntity(m_mapRender);
    addWorldEntity(m_regimentsRender);

    addHudEntity(m_hud);
  }

  void GameScene::doProcessEvent(gf::Event &event) {
    m_adaptor.processEvent(event);
    m_armySelection.processEvent(event);
  }

  void GameScene::doUpdate(gf::Time time) {
    gf::unused(time);
    m_gameState.update(time);

    processPackets();
  }

  void GameScene::processPackets() {
    PacketGameClient packet;
    while (gNetwork().receiveGamePacket(packet)) {
      switch (packet.type) {
        case PacketGameClientType::CreateRegiment:
          gf::Log::info("Regiment created: {count: %d, pos: %d,%d, owner: %lx}\n",
            packet.createRegiment.count, packet.createRegiment.position.x,
            packet.createRegiment.position.y, packet.createRegiment.ownerID);

          m_gameState.data.regiments.push_back({packet.createRegiment.ownerID, packet.createRegiment.count, packet.createRegiment.position, packet.createRegiment.division});
          break;

        case PacketGameClientType::MoveUnit:
          gf::Log::info("Move unit from {%d,%d} to {%d,%d}\n",
            packet.moveUnit.origin.x, packet.moveUnit.origin.y,
            packet.moveUnit.destination.x, packet.moveUnit.destination.y);

          m_gameState.data.moveUnit(packet.moveUnit.origin, packet.moveUnit.destination);
          break;

        case PacketGameClientType::KillUnit:
          gf::Log::info("Kill unit at {%d,%d}\n",
            packet.killUnit.position.x, packet.killUnit.position.y);

          m_gameState.data.killUnit(packet.killUnit.position);

          break;

        case PacketGameClientType::InitializePlayer:
          assert(packet.initializePlayer.playerID == m_gameState.currentPlayerID);

          gf::Log::info("Inital data: {playerID: %lx, pos: %d,%d}\n", packet.initializePlayer.playerID, packet.initializePlayer.position.x, packet.initializePlayer.position.y);
          setWorldViewCenter(Hexagon::positionToCoordinates(packet.initializePlayer.position));

          break;

        case PacketGameClientType::DeclareWinner:
          gf::Log::info("Winner playerID: %lx\n", packet.declareWinner.winner);

          break;
      }
    }
  }
}
