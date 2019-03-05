#include <iostream>

#include <gf/ResourceManager.h>

#include "bits/client/ClientModel.h"
#include "bits/client/GameStage.h"
#include "bits/client/LobbyStage.h"

#include "config.h"

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: global-war <host> <port>\n";
      return 1;
    }

    // Init screen
    gf::Window window("Global War", gw::InitialScreenSize);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    gf::RenderWindow renderer(window);

    // Define the model
    gw::ClientModel clientModel(argv[1], argv[2]);

    // Start the network
    clientModel.threadCom.start();

    // Assets manager
    gf::ResourceManager resources;
    resources.addSearchDir(GLOBAL_WAR_DATA_DIR);

    // Create stages
    gw::LobbyStage lobby(window, renderer, clientModel, resources);
    lobby.loop();

    gw::GameStage game(window, renderer, clientModel, resources);
    game.loop();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
