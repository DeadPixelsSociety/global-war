#include "GameSession.h"

#include <thread>

#include <gf/Clock.h>
#include <gf/Sleep.h>
#include <gf/Vector.h>

#include "Singletons.h"

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

    // Wait the acknowledge of all players
    for (std::size_t i = 0; i < m_players.size(); ++i) {
      Packet packet;
      m_queue.wait(packet);

      assert(packet.type == PacketType::AckJoinGame);
    }

    // Create the player's regiments
    auto createUnit = [&](Player &player, gf::Vector2i position, Division division) {
      // Add the new regiment to the model
      Regiment regiment;
      regiment.ownerID = player.getID();
      regiment.position = position;
      regiment.count = 40;
      regiment.division = division;
      m_gameState.data.regiments.push_back(regiment);

      // TODO: Remove this packet to move this an the first init of GameModel
      packet.type = PacketType::CreateRegiment;
      packet.createRegiment.count = regiment.count;
      packet.createRegiment.position = regiment.position;
      packet.createRegiment.ownerID = regiment.ownerID;
      packet.createRegiment.division = regiment.division;

      // Send this regiment at all players
      for (auto p: m_players) {
        p->sendPacket(packet);
      }
    };

    // Choose the initial location for players
    auto initialPositions = m_gameState.data.map.generateInitialPositions(gRandom(), m_players.size());

    for (std::size_t i = 0; i < m_players.size(); ++i) {
      auto neighborsPositions = m_gameState.data.map.getEmptyNeighborPositions(initialPositions[i]);
      assert(neighborsPositions.size() >= 3);

      createUnit(*m_players[i], neighborsPositions[0], Division::Lancer);
      createUnit(*m_players[i], neighborsPositions[1], Division::Swordsman);
      createUnit(*m_players[i], neighborsPositions[2], Division::Horseman);
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
          case PacketType::MoveRegiment:
          {
            if (!m_gameState.data.isValidMove(packet.moveRegiment.regimentOrigin, packet.moveRegiment.regimentDestination)) {
              break;
            }

            // If the order already exists
            if (std::find_if(m_gameState.moveOrders.begin(), m_gameState.moveOrders.end(), [packet](const MoveOrder& order){
              return packet.moveRegiment.regimentOrigin == order.origin && packet.moveRegiment.regimentDestination == order.destination;
            }) != m_gameState.moveOrders.end()) {
              break;
            }

            // Create a move order
            m_gameState.moveOrders.push_back({ packet.moveRegiment.regimentOrigin, packet.moveRegiment.regimentDestination, gf::Time::zero() });
            break;
          }

          case PacketType::NewPlayer:
          case PacketType::QuickMatch:
          case PacketType::JoinGame:
          case PacketType::AckJoinGame:
          case PacketType::CreateRegiment:
          case PacketType::MoveUnit:
          case PacketType::KillUnit:
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
