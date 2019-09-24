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
    PacketLobbyServer packet;

    // Read all pending packets
    while (networkManager.receiveLobbyPackets(packet)) {
      // Do somethings
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
