#include "Sockets.h"

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <thread>

#include <gf/Log.h>

using boost::asio::ip::tcp;

namespace gw {

  SocketTcp::SocketTcp(boost::asio::ip::tcp::socket socket)
  : m_socket(std::move(socket)) {

  }

  SocketTcp::SocketTcp(const char* server, const char* port)
  : m_socket(m_ioService) {
    tcp::resolver resolver(m_ioService);
    boost::asio::connect(m_socket, resolver.resolve({ server, port }));
  }

  SocketTcp::SocketTcp(SocketTcp&& other)
  : m_socket(std::move(other.m_socket)) {

  }

  SocketTcp& SocketTcp::operator=(SocketTcp&& other) {
    m_socket = std::move(other.m_socket);

    return *this;
  }

  ListenerTcp::ListenerTcp(std::uint16_t port)
  : m_acceptor(m_ioService, tcp::endpoint(tcp::v4(), port)) {

  }

  void ListenerTcp::waitNewConnection(std::function<void(boost::asio::ip::tcp::socket)> handler) {
    // Accept the new connection
    tcp::socket socket(m_ioService);
    m_acceptor.accept(socket);

    // Call the handler
    handler(std::move(socket));
  }
}
