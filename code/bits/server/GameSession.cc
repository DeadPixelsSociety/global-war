#include "GameSession.h"

#include <thread>

namespace gw {
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

      packet.createRegiment.count = 40;
      packet.createRegiment.position = { 0, static_cast<int>(i) };
      packet.createRegiment.ownerID = player.getID();

      // Send this regiment at all players
      for (auto p: m_players) {
        p->sendPacket(packet);
      }
    }
  }

  void GameSession::launchGame() {
    std::thread([this](){
      Packet packet;
      m_queue.wait(packet);

      switch (packet.type) {
      case PacketType::Ping:
        gf::Log::info("Ping: %d\n", packet.ping.sequence);
        break;

      case PacketType::NewPlayer:
        gf::Log::info("Player ID: %lx\n", packet.newPlayer.playerID);
        break;

      case PacketType::QuickMatch:
        gf::Log::info("Player ID: %lx\n", packet.quickMatch.playerID);
        break;

      case PacketType::JoinGame:
        gf::Log::info("Game ID: %lx\n", packet.joinGame.gameID);
        break;

      case PacketType::CreateRegiment:
      case PacketType::MoveRegiment:
        assert(false);
        break;
      }
    }).detach();
  }
}
