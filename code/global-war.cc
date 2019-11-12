#include <iostream>
#include <thread>
#include <chrono>

#include <gf/SceneManager.h>
#include <gf/ResourceManager.h>
#include <gf/Unused.h>

#include <gf/Log.h>

#include "bits/client/ClientMessages.h"
#include "bits/client/GameScene.h"
// #include "bits/client/GameState.h"
#include "bits/client/LobbyScene.h"
#include "bits/client/NetworkManagerClient.h"
#include "bits/client/Singletons.h"

#include "bits/common/Packets.h"
#include "bits/common/Sockets.h"

#include "config.h"

int main(int argc, char *argv[]) {
  #if 0
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

  #endif // 0
  if (argc != 4) {
    std::cerr << "Usage: global-war <host> <port_lobby> <port_game>\n";
    return 1;
  }

  // Network manager
  gf::SingletonStorage<gw::NetworkManagerClient> storageForNetwork(gw::gNetwork, argv[1], argv[2], argv[3]);

  // Message manager
  gf::SingletonStorage<gf::MessageManager> storageForMessageManager(gw::gMessageManager);

  // Create the scene manager and the opengl context
  gf::SceneManager sceneManager("Global War", gw::InitialScreenSize);

  // Assets manager
  gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(gw::gResourceManager);
  gw::gResourceManager().addSearchDir(GLOBAL_WAR_DATA_DIR);


  // Define the model
  gw::GameState gameState;

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

  return 0;
}
