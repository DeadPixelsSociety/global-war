#include "Lobby.h"

#include <gf/Log.h>

#include "Messages.h"
#include "Singletons.h"

namespace gw {
  static constexpr int GameLimitPlayer = 2;

  Lobby::Lobby() {
    gMessageManager().registerHandler<PlayerInLobby>(&Lobby::onPlayerInLobby, this);
  }

  void Lobby::processPackets(NetworkManagerServer& networkManager) {
    std::lock_guard<std::mutex> lock(m_lobbyVectorMutex);
    // Read all pending packets
    for (auto playerID: m_lobbyPlayers) {
      PacketLobbyServer packet;

      while (networkManager.receiveLobbyPacket(playerID, packet)) {
        // Do somethings
      }
    }
  }

  gf::MessageStatus Lobby::onPlayerInLobby(gf::Id id, gf::Message *msg) {
      assert(id == PlayerInLobby::type);
      PlayerInLobby *playerInLobby = static_cast<PlayerInLobby*>(msg);

      {
        std::lock_guard<std::mutex> lock(m_lobbyVectorMutex);
        m_lobbyPlayers.push_back(playerInLobby->playerID);
      }

      return gf::MessageStatus::Keep;
  }
}
