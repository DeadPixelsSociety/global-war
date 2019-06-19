#include <iostream>

#include <gf/SceneManager.h>
#include <gf/ResourceManager.h>

#include "bits/client/GameScene.h"
#include "bits/client/GameState.h"
#include "bits/client/LobbyScene.h"
#include "bits/client/Singletons.h"

#include "config.h"

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: global-war <host> <port>\n";
      return 1;
    }


    // Init screen
    // gf::Window window("Global War", gw::InitialScreenSize);
    // window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(60);
    // gf::RenderWindow renderer(window);

    // Assets manager
    gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(gw::gResourceManager);
    gw::gResourceManager().addSearchDir(GLOBAL_WAR_DATA_DIR);

    // Define the model
    gw::GameState gameState(argv[1], argv[2]);

    // Start the network
    gameState.threadCom.start();

    // Create scenes
    gw::LobbyScene lobbyScene(gw::InitialScreenSize, gameState);
    gw::GameScene gameScene(gw::InitialScreenSize, gameState);
    gameScene.pause();
    gameScene.hide();

    gf::SceneManager sceneManager("Global War", gw::InitialScreenSize);
    sceneManager.pushScene(lobbyScene);
    // sceneManager.pushScene(gameScene);

    sceneManager.run();

    // gw::LobbyStage lobby(window, renderer, gameState);
    // gw::GameStage game(window, renderer, gameState);

    // Start the loops
    // lobby.loop();
    // game.loop();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
