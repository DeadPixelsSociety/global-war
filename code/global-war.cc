#include <iostream>

#include <gf/ResourceManager.h>

#include "bits/client/GameStage.h"
#include "bits/client/GameState.h"
#include "bits/client/LobbyStage.h"
#include "bits/client/Singletons.h"

#include "config.h"

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: global-war <host> <port>\n";
      return 1;
    }

    // Assets manager
    gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(gw::gResourceManager);
    gw::gResourceManager().addSearchDir(GLOBAL_WAR_DATA_DIR);

    // Init screen
    gf::Window window("Global War", gw::InitialScreenSize);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    gf::RenderWindow renderer(window);

    // Define the model
    gw::GameState gameState(argv[1], argv[2]);

    // Start the network
    gameState.threadCom.start();

    // Create stages
    gw::LobbyStage lobby(window, renderer, gameState);
    lobby.loop();

    gw::GameStage game(window, renderer, gameState);
    game.loop();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
