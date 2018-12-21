#ifndef BITS_SERVER_CONNECTION_HANDLER
#define BITS_SERVER_CONNECTION_HANDLER

#include <vector>

#include <boost/asio.hpp>

#include <gf/Queue.h>

#include "../common/Packet.h"

#include "PlayerCom.h"

namespace gw {
  class ConnectionHandler {
  public:
    ConnectionHandler(std::uint16_t port);

    void waitNewPlayers();

  private:
    std::uint16_t m_port;
    boost::asio::io_service m_ioService;
    gf::Queue<Packet> m_comQueue;
    std::vector<PlayerCom> m_players;
  };
}

#endif // BITS_SERVER_CONNECTION_HANDLER
