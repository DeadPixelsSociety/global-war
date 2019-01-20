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
    }

    gf::Log::info("New game session created with %lu players\n", m_players.size());
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
        gf::Id playerID = packet.quickMatch.playerID;
        gf::Log::info("Player ID: %lx\n", playerID);
        break;
      }
    }).detach();
  }
}
