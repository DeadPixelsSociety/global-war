#ifndef BITS_SERVER_LOBBY
#define BITS_SERVER_LOBBY

#include <map>
#include <mutex>
#include <queue>

#include <gf/Id.h>
#include <gf/Message.h>

#include "NetworkManagerServer.h"
#include "Utils.h"

namespace gw {
  class Lobby {
  public:
    Lobby();

    void processPackets(NetworkManagerServer& networkManager);
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
    std::map<gf::Id, PlayerState> m_lobbyPlayers;
    std::queue<gf::Id> m_waitQueue;
    std::mutex m_lobbyVectorMutex;
  };
}

#endif // BITS_SERVER_LOBBY
