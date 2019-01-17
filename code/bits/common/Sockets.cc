#include "Sockets.h"

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <thread>

#include <gf/Log.h>
#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

using boost::asio::ip::tcp;

namespace gw {
  SocketTcp::SocketTcp()
  : m_socket(m_ioService)
  , m_state(SocketState::Disconnected) {
  }

  SocketTcp::SocketTcp(boost::asio::ip::tcp::socket socket)
  : m_socket(std::move(socket))
  , m_state(SocketState::Connected) {

  }

  SocketTcp::SocketTcp(SocketTcp&& other)
  : m_socket(std::move(other.m_socket))
  , m_state(other.m_state) {

  }

  SocketTcp& SocketTcp::operator=(SocketTcp&& other) {
    m_socket = std::move(other.m_socket);
    m_state = other.m_state;

    return *this;
  }

  SocketState SocketTcp::getState() const {
    return m_state;
  }

  void SocketTcp::connectTo(const char* server, const char* port) {
    tcp::resolver resolver(m_ioService);
    boost::asio::connect(m_socket, resolver.resolve({ server, port }));
    m_state = SocketState::Connected;
  }

  void SocketTcp::send(Packet &packet) {
    constexpr int MaxLength = 1024;
    uint8_t data[MaxLength];

    gf::MemoryOutputStream stream(data);
    gf::Serializer ar(stream);

    ar | packet;

    boost::asio::write(m_socket, boost::asio::buffer(data, MaxLength));
  }

  void SocketTcp::receive(Packet &packet) {
    constexpr int MaxLength = 1024;
    uint8_t data[MaxLength];

    boost::system::error_code error;

    size_t length = m_socket.read_some(boost::asio::buffer(data), error);

    if (error == boost::asio::error::eof) {
      m_state = SocketState::Disconnected;
      return; // Connection closed cleanly by peer.
    } else if (error) {
      throw boost::system::system_error(error); // Some other error.
    }

    gf::MemoryInputStream stream(gf::ArrayRef<uint8_t>(data, length));
    gf::Deserializer ar(stream);

    ar | packet;
  }

  ListenerTcp::ListenerTcp(std::uint16_t port)
  : m_acceptor(m_ioService, tcp::endpoint(tcp::v4(), port)) {

  }

  void ListenerTcp::handleNewConnection(std::function<void(SocketTcp)> handler) {
    std::thread([this, handler](){
      for(;;) {
        tcp::socket socket(m_ioService);
        m_acceptor.accept(socket);

        SocketTcp wrapper(std::move(socket));

        handler(std::move(wrapper));
      }
    }).detach();
  }
}
