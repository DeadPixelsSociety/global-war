#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include <gf/Streams.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>

#include "bits/common/Packet.h"


using boost::asio::ip::tcp;

namespace {

  constexpr int MaxLength = 1024;

  void session(tcp::socket sock) {
    try {
      for (;;) {
        uint8_t data[MaxLength];

        boost::system::error_code error;

        size_t length = sock.read_some(boost::asio::buffer(data), error);

        if (error == boost::asio::error::eof) {
          break; // Connection closed cleanly by peer.
        } else if (error) {
          throw boost::system::system_error(error); // Some other error.
        }

        gf::MemoryInputStream stream(gf::ArrayRef<uint8_t>(data, length));
        gf::Deserializer ar(stream);

        gw::Packet packet;
        ar | packet;

        assert(packet.type == gw::PacketType::Ping);

        std::cout << "Ping: " << packet.ping.sequence << '\n';
      }
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception in thread: " << e.what() << "\n";
    }
  }

  void server(boost::asio::io_service& io_service, unsigned short port) {
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));

    for (;;) {
      tcp::socket sock(io_service);
      a.accept(sock);
      std::thread(session, std::move(sock)).detach();
    }
  }

}


int main(int argc, char *argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: global-war-server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;
    server(io_service, std::atoi(argv[1]));

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
  }

  return 0;

}
