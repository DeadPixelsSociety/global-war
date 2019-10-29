#ifndef BITS_SERVER_LOBBY
#define BITS_SERVER_LOBBY

#include <map>
#include <mutex>
#include <queue>

#include <gf/Id.h>
#include <gf/Message.h>

#include "GameSession.h"
#include "NetworkManagerServer.h"
#include "Utils.h"

namespace gw {
  class Lobby {
  public:
    Lobby(NetworkManagerServer& network);

    void processPackets();
    void update();

  private:
    gf::MessageStatus onPlayerInLobby(gf::Id id, gf::Message *msg);

    void createNewGame();

  private:
    enum class PlayerState {
      Idling,
      WaitingMatch,
      Playing,
    };

  private:
    NetworkManagerServer& m_network;
    std::map<gf::Id, PlayerState> m_lobbyPlayers;
    std::queue<gf::Id> m_waitQueue;
    std::mutex m_lobbyVectorMutex;
    std::queue<PacketLobbyClient> m_pendingPackets;
    std::map<gf::Id, GameSession> m_games;
  };
}

#endif // BITS_SERVER_LOBBY
