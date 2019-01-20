#include "GameSession.h"

namespace gw {
  GameSession::GameSession(gf::Id gameID, std::map<gf::Id, Player> &players)
  : m_gameID(gameID) {
    for (auto &player: players) {
      if (player.second.getState() == Player::State::WaitingMatch) {
        player.second.updateQueue(&m_queue);
        m_players.push_back(&player.second);
      }
    }

    gf::Log::info("New game session created with %lu players\n", m_players.size());
  }
}
