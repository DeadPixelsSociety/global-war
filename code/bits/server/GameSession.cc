#include "GameSession.h"

#include <thread>

#include <gf/Clock.h>
#include <gf/Sleep.h>

namespace gw {
  namespace {

    constexpr gf::Time TickTime = gf::seconds(1 / 120.0f);

  }

  GameSession::GameSession(gf::Id gameID, std::map<gf::Id, Player> &players)
  : m_gameID(gameID) {
    for (auto &player: players) {
      if (player.second.getState() == Player::State::WaitingMatch) {
        player.second.updateQueue(&m_queue);
        player.second.plays();
        m_players.push_back(&player.second);
      }

      // Sort player by ID
      std::sort(m_players.begin(), m_players.end(), [](Player *rhs, Player *lhs){
        return rhs->getID() < lhs->getID();
      });
    }

    gf::Log::info("New game session created with %lu players\n", m_players.size());

    // Send to clients the game info
    Packet packet;
    packet.type = PacketType::JoinGame;
    packet.joinGame.gameID = m_gameID;
    packet.joinGame.nbPlayers = m_players.size();

    // Add all player IDs
    for (std::size_t i = 0; i < m_players.size(); ++i) {
      packet.joinGame.playersID[i] = m_players[i]->getID();
    }

    // Send packet at all client
    for (Player *player: m_players) {
      player->sendPacket(packet);
    }

    // Create the player's regiments
    packet.type = PacketType::CreateRegiment;
    for (std::size_t i = 0; i < m_players.size(); ++i) {
      Player &player = *m_players[i];

      // Add the new regiment to the model
      Regiment regiment;
      regiment.ownerID = player.getID();
      regiment.position = { 0, static_cast<int>(i) };
      regiment.count = 40;
      m_gameState.data.regiments.push_back(regiment);

      // TODO: Remove this packet to move this an the first init of GameModel
      packet.createRegiment.count = regiment.count;
      packet.createRegiment.position = regiment.position;
      packet.createRegiment.ownerID = regiment.ownerID;

      // Send this regiment at all players
      for (auto p: m_players) {
        p->sendPacket(packet);
      }
    }
  }

  void GameSession::launchGame() {
    std::thread([this](){
      gf::Clock clock;

      for(;;) {
        // Process pending packet
        Packet packet;
        while (m_queue.poll(packet)) {
          switch (packet.type) {
          case PacketType::NewPlayer:
            gf::Log::info("Player ID: %lx\n", packet.newPlayer.playerID);
            break;

          case PacketType::QuickMatch:
            gf::Log::info("Player ID: %lx\n", packet.quickMatch.playerID);
            break;

          case PacketType::JoinGame:
            gf::Log::info("Game ID: %lx\n", packet.joinGame.gameID);
            break;

          case PacketType::MoveRegiment:
          {
            // TODO: check if the position is valid

            // Create a move order
            m_gameState.moveOrders.push_back({ packet.moveRegiment.regimentOrigin, packet.moveRegiment.regimentDestination, gf::Time::zero() });
            break;
          }

          case PacketType::CreateRegiment:
          case PacketType::MoveUnit:
            assert(false);
            break;
          }
        }

        // Update model
        gf::Time time = clock.restart();
        m_gameState.update(time);

        // Send all packets
        for (auto &packet: m_gameState.pendingPackets) {
          sendAtPlayers(packet);
        }
        m_gameState.pendingPackets.clear();

        if (time < TickTime) {
          gf::sleep(TickTime - time);
        }

      }
    }).detach();
  }

  void GameSession::sendAtPlayers(Packet &packet) {
    for (Player *p: m_players) {
      p->sendPacket(packet);
    }
  }
}
