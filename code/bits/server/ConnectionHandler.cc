#include "ConnectionHandler.h"

using boost::asio::ip::tcp;

namespace gw {
  ConnectionHandler::ConnectionHandler(std::uint16_t port)
  : m_port(port){
  }

  void ConnectionHandler::waitNewPlayers() {
    tcp::acceptor a(m_ioService, tcp::endpoint(tcp::v4(), m_port));

    // std::vector<gw::ComThread> threads;

    for (;;) {
      tcp::socket sock(m_ioService);
      a.accept(sock);

      // Create a new player
      m_players.emplace_back(std::move(sock));
    }
  }
}
