#include <iostream>

#include <gf/Random.h>

#include "bits/server/Lobby.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
  gf::Random random;

  try {
    if (argc != 2) {
      std::cerr << "Usage: global-war-server <port>\n";
      return 1;
    }
    gw::Lobby lobby(random, std::atoi(argv[1]));

    for (;;) {
      lobby.processPacket();
    }

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
  }

  return 0;

}
