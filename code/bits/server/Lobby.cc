#include "Lobby.h"

#include <gf/Log.h>

#include "Messages.h"
#include "Singletons.h"

namespace gw {
  static constexpr int GameLimitPlayer = 2;

  Lobby::Lobby(NetworkManagerServer& network)
  : m_network(network){
    gMessageManager().registerHandler<PlayerInLobby>(&Lobby::onPlayerInLobby, this);
  }

  void Lobby::processPackets() {
    // Read all pending packets
    PacketLobbyServer packet;
    while (m_network.receiveLobbyPackets(packet)) {
      switch (packet.type) {
        case PacketLobbyServerType::RequestMatch:
          assert(packet.playerID == packet.requestMatch.playerID);

          gf::Log::info("Player #%lx request match\n", packet.requestMatch.playerID);

          assert(m_lobbyPlayers.at(packet.playerID) == PlayerState::Idling);
          m_lobbyPlayers.at(packet.playerID) = PlayerState::WaitingMatch;

          m_waitQueue.push(packet.playerID);

          break;
        case PacketLobbyServerType::ConfirmJoinGame:
          auto &gameEntry = m_games.at(packet.confirmJoinGame.gameID);
          m_lobbyPlayers.at(packet.playerID) = PlayerState::Playing;

          gameEntry.ackPlayer();
          break;
      }
    }

    // Send all pending packets
    while (!m_pendingPackets.empty()) {
      m_network.sendLobbyPacket(m_pendingPackets.front());
      m_pendingPackets.pop();
    }
  }

  gf::MessageStatus Lobby::onPlayerInLobby(gf::Id id, gf::Message *msg) {
    assert(id == PlayerInLobby::type);
    PlayerInLobby *playerInLobby = static_cast<PlayerInLobby*>(msg);

    {
      std::lock_guard<std::mutex> lock(m_lobbyVectorMutex);

      auto result = m_lobbyPlayers.insert({playerInLobby->playerID, PlayerState::Idling});
      assert(result.second);
    }

    return gf::MessageStatus::Keep;
  }

  void Lobby::update() {
    // Launch a game session when enough ready players
    if (m_waitQueue.size() >= GameLimitPlayer) {
      createNewGame();
    }
  }

  void Lobby::createNewGame() {
    // Start game
    gf::Id gameID = generateId();

    // Get the players and change their status
    std::vector<gf::Id> players;
    for (int i = 0; i < GameLimitPlayer; ++i) {
      gf::Id playerID = m_waitQueue.front();
      players.push_back(playerID);
      m_lobbyPlayers.at(playerID) = PlayerState::Playing;
      m_waitQueue.pop();
    }

    gf::Log::info("Game ID: %lx\n", gameID);
    gf::Log::info("Number players: %d\n", GameLimitPlayer);
    for (int i = 0; i < GameLimitPlayer; ++i) {
      gf::Log::info("Players[%d] ID = %lx\n", i, players[i]);
    }

    // gf::Log::debug("Waiting queue size %lu", m_waitQueue.size());

    // Send the packet to the players
    PacketLobbyClient packet;
    packet.type = PacketLobbyClientType::JoinGame;

    for (int i = 0; i < GameLimitPlayer; ++i) {
      packet.playerID = players[i];

      packet.joinGame.gameID = gameID;
      packet.joinGame.nbPlayers = GameLimitPlayer;

      for (int j = 0; j < GameLimitPlayer; ++j) {
        packet.joinGame.playersID[j] = players[j];
      }

      m_pendingPackets.push(packet);
    }

    // Create new Session game
    bool inserted;
    decltype(m_games)::iterator it;

    std::tie(it, inserted) = m_games.emplace(std::piecewise_construct,
      std::forward_as_tuple(gameID),
      std::forward_as_tuple(m_network, gameID, players));

    assert(inserted);
  }
}
