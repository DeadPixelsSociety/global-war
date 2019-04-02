#ifndef BITS_SERVER_LOBBY
#define BITS_SERVER_LOBBY

#include <vector>
#include <map>

#include <gf/Id.h>
#include <gf/Queue.h>
#include <gf/Random.h>

#include "../common/Packet.h"
#include "../common/Sockets.h"

#include "GameSession.h"
#include "Player.h"

namespace gw {
  class Lobby {
  public:
    Lobby(std::uint16_t port);

    void addNewPlayer(SocketTcp socket);
    void processPacket();

  private:
    gf::Id generateId() const;
    void createNewGame();

  private:
    ListenerTcp m_listener;

    gf::Queue<Packet> m_comQueue;

    std::mutex m_playerMutex;
    std::map<gf::Id, Player> m_players;

    std::map<gf::Id, GameSession> m_games;
  };
}

#endif // BITS_SERVER_LOBBY
