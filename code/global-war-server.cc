#include <iostream>

#include "bits/server/ConnectionHandler.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: global-war-server <port>\n";
      return 1;
    }
    gw::ConnectionHandler server(std::atoi(argv[1]));

    for (;;) {
      server.processPacket();
    }

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
  }

  return 0;

}
