#ifndef BITS_SERVER_LOBBY
#define BITS_SERVER_LOBBY

#include <mutex>
#include <vector>

#include <gf/Id.h>
#include <gf/Message.h>

#include "NetworkManagerServer.h"

namespace gw {
  class Lobby {
  public:
    Lobby();

    void processPackets(NetworkManagerServer& networkManager);

  private:
    gf::MessageStatus onPlayerInLobby(gf::Id id, gf::Message *msg);

  private:
    std::vector<gf::Id> m_lobbyPlayers;
    std::mutex m_lobbyVectorMutex;
  };
}

#endif // BITS_SERVER_LOBBY
