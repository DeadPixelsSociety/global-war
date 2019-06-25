#include "LobbyScene.h"

#include <gf/Log.h>
#include <gf/Unused.h>

#include "ClientMessages.h"
#include "Singletons.h"

namespace gw {
  LobbyScene::LobbyScene(const gf::Vector2i &initializeSize, GameState &gameState)
  : Scene(initializeSize)
  , m_gameState(gameState)
  , m_waitScreen(gResourceManager().getFont("DejaVuSans.ttf")) {
    addHudEntity(m_waitScreen);

    // Register for a quick match
    m_gameState.quickMatch();
  }

  void LobbyScene::doUpdate(gf::Time time) {
    gf::unused(time);
    processPackets();
  }

  void LobbyScene::processPackets() {
    Packet packet;
    while (m_gameState.comQueue.poll(packet)) {
      switch (packet.type) {
        case PacketType::NewPlayer:
        case PacketType::QuickMatch:
        case PacketType::AckJoinGame:
        case PacketType::CreateRegiment:
        case PacketType::MoveRegiment:
        case PacketType::MoveUnit:
        case PacketType::KillUnit:
        case PacketType::InitializePlayer:
        case PacketType::WinGame:
          gf::Log::error("Receive unexpected packet in lobby\n");
          assert(false);
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

          // Send the acknowledge to join session game
          Packet packet;
          packet.type = PacketType::AckJoinGame;
          bool ok = m_gameState.threadCom.sendPacket(packet);
          assert(ok);

          // Disable the scene
          GameStart gameStart;
          gMessageManager().sendMessage(&gameStart);
          // hide();
          // pause();
          // setActive(false);
          break;
      }
    }
  }
}
