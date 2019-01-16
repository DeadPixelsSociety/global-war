#ifndef BITS_SERVER_LOBBY
#define BITS_SERVER_LOBBY

#include <vector>
#include <map>

#include <boost/asio.hpp>

#include <gf/Id.h>
#include <gf/Queue.h>
#include <gf/Random.h>

#include "../common/Packet.h"

#include "PlayerCom.h"

namespace gw {
  class Lobby {
  public:
    Lobby(gf::Random &random, std::uint16_t port);

    void waitNewPlayers();
    void processPacket();

  private:
    gf::Id generateId() const;

  private:
    gf::Random& m_random;
    std::uint16_t m_port;

    gf::Queue<Packet> m_comQueue;

    std::mutex m_playerMutex;
    std::map<gf::Id, PlayerCom> m_players;
  };
}

#endif // BITS_SERVER_LOBBY
