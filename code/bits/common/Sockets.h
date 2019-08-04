#ifndef GW_COMMON_SOCKETS_H
#define GW_COMMON_SOCKETS_H

#include <functional>

#include <boost/asio.hpp>

#include <gf/Log.h>
#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

#include "Packets.h"

namespace gw {
  class SocketTcp {
  public:
    SocketTcp(boost::asio::ip::tcp::socket socket); // Server side
    SocketTcp(const char* server, const char* port); // Client side

    SocketTcp(const SocketTcp&) = delete;
    SocketTcp& operator=(const SocketTcp&) = delete;

    SocketTcp(SocketTcp&& other);
    SocketTcp& operator=(SocketTcp&& other);

    template <typename Packet>
    bool send(Packet &packet) {
      constexpr int MaxLength = 1024;
      uint8_t data[MaxLength] = { 0 };

      gf::MemoryOutputStream stream(data);
      gf::Serializer ar(stream);

      ar | packet;

      boost::system::error_code error;
      boost::asio::write(m_socket, boost::asio::buffer(data, MaxLength), error);

      if (error == boost::asio::error::eof) {
        // TODO: How to handle the disconnection ? @ahugeat
        return false; // Connection closed cleanly by peer
      } else if (error) {
        throw boost::system::system_error(error);
        return false;
      }

      return true;
    }

    template <typename Packet>
    bool receive(Packet &packet) {
      constexpr int MaxLength = 1024;
      uint8_t data[MaxLength] = { 0 };

      boost::system::error_code error;
      size_t length = m_socket.read_some(boost::asio::buffer(data), error);

      if (error == boost::asio::error::eof) {
        // TODO: How to handle the disconnection ? @ahugeat
        return false; // Connection closed cleanly by peer
      } else if (error) {
        throw boost::system::system_error(error);
        return false;
      }

      gf::MemoryInputStream stream(gf::ArrayRef<uint8_t>(data, length));
      gf::Deserializer ar(stream);

      ar | packet;

      return true;
    }

  private:
    boost::asio::io_service m_ioService; // Move to static ? @ahugeat
    boost::asio::ip::tcp::socket m_socket;
  };

  class ListenerTcp {
  public:
    ListenerTcp(std::uint16_t port);

    ListenerTcp(const SocketTcp&) = delete;
    ListenerTcp& operator=(const SocketTcp&) = delete;

    ListenerTcp(SocketTcp&& other) = delete;
    ListenerTcp& operator=(SocketTcp&& other) = delete;

    void waitNewConnection(std::function<void(boost::asio::ip::tcp::socket)> handler);

  private:
    boost::asio::io_service m_ioService;
    boost::asio::ip::tcp::acceptor m_acceptor;
  };
}

#endif // GW_COMMON_SOCKETS_H
