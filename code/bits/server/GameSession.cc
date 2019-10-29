#include "GameSession.h"

#include <thread>

#include <gf/Clock.h>
#include <gf/Log.h>
#include <gf/Sleep.h>
#include <gf/Vector.h>

#include "Singletons.h"

namespace gw {
  namespace {

    constexpr gf::Time TickTime = gf::seconds(1 / 120.0f);
    constexpr int IntialRegimentSize = 1;

  }

  GameSession::GameSession(NetworkManagerServer& network, gf::Id gameID, std::vector<gf::Id> players)
  : m_network(network)
  , m_gameID(gameID)
  , m_players(players)
  , m_waitingAck(m_players.size()) {
    // // Sort player by ID
    // std::sort(m_players.begin(), m_players.end(), [](Player *rhs, Player *lhs){
    //   return rhs->getID() < lhs->getID();
    // });

    gf::Log::info("New game session (#%lx) created with %lx players\n", m_gameID, m_players.size());

    // Wait the acknowledge of all players -> to lobby
    // for (std::size_t i = 0; i < m_players.size(); ++i) {
    //   Packet packet;
    //   m_queue.wait(packet);
    //
    //   assert(packet.type == PacketType::AckJoinGame);
    // }

    // // Create the player's regiments -> to other method
    // auto createUnit = [&](Player &player, gf::Vector2i position, Division division) {
    //   // Add the new regiment to the model
    //   Regiment regiment;
    //   regiment.ownerID = player.getID();
    //   regiment.position = position;
    //   regiment.count = IntialRegimentSize;
    //   regiment.division = division;
    //   m_gameState.data.regiments.push_back(regiment);
    //
    //   // TODO: Remove this packet to move this an the first init of GameModel
    //   packet.type = PacketType::CreateRegiment;
    //   packet.createRegiment.count = regiment.count;
    //   packet.createRegiment.position = regiment.position;
    //   packet.createRegiment.ownerID = regiment.ownerID;
    //   packet.createRegiment.division = regiment.division;
    //
    //   // Send this regiment at all players
    //   for (auto p: m_players) {
    //     p->sendPacket(packet);
    //   }
    // };
    //
    // // Choose the initial location for players
    // auto initialPositions = m_gameState.data.map.generateInitialPositions(gRandom(), m_players.size());
    //
    // for (std::size_t i = 0; i < m_players.size(); ++i) {
    //   auto neighborsPositions = m_gameState.data.map.getEmptyNeighborPositions(initialPositions[i]);
    //   assert(neighborsPositions.size() >= 3);
    //
    //   createUnit(*m_players[i], neighborsPositions[0], Division::Lancer);
    //   createUnit(*m_players[i], neighborsPositions[1], Division::Swordsman);
    //   createUnit(*m_players[i], neighborsPositions[2], Division::Horseman);
    //
    //   packet.type = PacketType::InitializePlayer;
    //   packet.initializePlayer.playerID = m_players[i]->getID();
    //   packet.initializePlayer.position = initialPositions[i];
    //
    //   m_players[i]->sendPacket(packet);
    // }

  }

  void GameSession::ackPlayer() {
    --m_waitingAck;

    if (m_waitingAck == 0) {
      gf::Log::debug("Launch game\n");
    }
  }

  // void GameSession::launchGame() {
  //   std::thread([this](){
  //     // Create the player ID list
  //     std::vector<gf::Id> playerIDs;
  //
  //     for (auto *player: m_players) {
  //       playerIDs.push_back(player->getID());
  //     }
  //
  //     gf::Clock clock;
  //
  //     for(;;) {
  //       // Process pending packet
  //       Packet packet;
  //       while (m_queue.poll(packet)) {
  //         switch (packet.type) {
  //         case PacketType::MoveRegiment:
  //         {
  //           if (!m_gameState.data.isValidMove(packet.moveRegiment.regimentOrigin, packet.moveRegiment.regimentDestination)) {
  //             break;
  //           }
  //
  //           // If the order already exists
  //           if (std::find_if(m_gameState.moveOrders.begin(), m_gameState.moveOrders.end(), [packet](const MoveOrder& order){
  //             return packet.moveRegiment.regimentOrigin == order.origin && packet.moveRegiment.regimentDestination == order.destination;
  //           }) != m_gameState.moveOrders.end()) {
  //             break;
  //           }
  //
  //           // Create a move order
  //           m_gameState.moveOrders.push_back({ packet.moveRegiment.regimentOrigin, packet.moveRegiment.regimentDestination, gf::Time::zero() });
  //           break;
  //         }
  //
  //         case PacketType::NewPlayer:
  //         case PacketType::QuickMatch:
  //         case PacketType::JoinGame:
  //         case PacketType::AckJoinGame:
  //         case PacketType::CreateRegiment:
  //         case PacketType::MoveUnit:
  //         case PacketType::KillUnit:
  //         case PacketType::InitializePlayer:
  //         case PacketType::WinGame:
  //           assert(false);
  //           break;
  //         }
  //       }
  //
  //       // Update model
  //       gf::Time time = clock.restart();
  //       m_gameState.update(time);
  //
  //       // Check the end condition
  //       m_gameState.checkEndCondition(playerIDs);
  //
  //       // Send all packets
  //       for (auto &packet: m_gameState.pendingPackets) {
  //         sendAtPlayers(packet);
  //       }
  //       m_gameState.pendingPackets.clear();
  //
  //       if (time < TickTime) {
  //         gf::sleep(TickTime - time);
  //       }
  //
  //     }
  //   }).detach();
  // }
  //
  // void GameSession::sendAtPlayers(Packet &packet) {
  //   for (Player *p: m_players) {
  //     p->sendPacket(packet);
  //   }
  // }
}
