#include <iostream>

#include "bits/client/GameState.h"

#include "config.h"

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: global-war <host> <port>\n";
      return 1;
    }

    // Assets manager
    gf::ResourceManager resources;
    resources.addSearchDir(GLOBAL_WAR_DATA_DIR);

    gw::GameState game(argv[1], argv[2], resources);

    game.loop();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
