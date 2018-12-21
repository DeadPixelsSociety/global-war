#ifndef BITS_SERVER_LOBBY
#define BITS_SERVER_LOBBY

#include <vector>
#include <mutex>

#include <boost/asio.hpp>

#include "../common/Packet.h"
#include "../common/Queue.h"

#include "PlayerCom.h"

namespace gw {
  class Lobby {
  public:
    Lobby(std::uint16_t port);

    void waitNewPlayers();
    void processPacket();

  private:
    std::uint16_t m_port;

    Queue m_comQueue;

    std::mutex m_playerMutex;
    std::vector<PlayerCom> m_players;
  };
}

#endif // BITS_SERVER_LOBBY
