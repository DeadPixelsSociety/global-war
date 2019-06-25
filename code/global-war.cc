#include <iostream>

#include <gf/SceneManager.h>
#include <gf/ResourceManager.h>
#include <gf/Unused.h>

#include "bits/client/ClientMessages.h"
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

    gf::SingletonStorage<gf::MessageManager> storageForMessageManager(gw::gMessageManager);

    // Create the scene manager and the opengl context
    gf::SceneManager sceneManager("Global War", gw::InitialScreenSize);

    // Assets manager
    gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(gw::gResourceManager);
    gw::gResourceManager().addSearchDir(GLOBAL_WAR_DATA_DIR);

    // Define the model
    gw::GameState gameState(argv[1], argv[2]);

    // Start the network
    gameState.threadCom.start();

    // Create scenes
    gw::LobbyScene lobbyScene(gw::InitialScreenSize, gameState);
    sceneManager.pushScene(lobbyScene);

    gw::GameScene gameScene(gw::InitialScreenSize, gameState, sceneManager.getRenderer());

    gw::gMessageManager().registerHandler<gw::GameStart>([&](gf::Id id, gf::Message *msg) {
      assert(id == gw::GameStart::type);
      gf::unused(msg);

      sceneManager.replaceScene(gameScene);

      return gf::MessageStatus::Keep;
    });

    // Launch the scenes
    sceneManager.run();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
