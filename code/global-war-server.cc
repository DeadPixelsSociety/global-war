#include <iostream>

#include <gf/Clock.h>
#include <gf/Random.h>
#include <gf/Sleep.h>

#include "bits/common/Constants.h"

#include "bits/server/Lobby.h"
#include "bits/server/Singletons.h"
#include "bits/server/NetworkManagerServer.h"

#include "config.h"

// using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
  #if 0
  // Init singleton
  gf::SingletonStorage<gf::Random> storageForRandom(gw::gRandom);

  gf::SingletonStorage<gf::AssetManager> storageForResourceManager(gw::gAssetManager);
  gw::gAssetManager().addSearchDir(GLOBAL_WAR_DATA_DIR);


  try {
    if (argc != 2) {
      std::cerr << "Usage: global-war-server <port>\n";
      return 1;
    }
    gw::Lobby lobby(std::atoi(argv[1]));

    for (;;) {
      lobby.processPacket();
    }

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << '\n';
  }

  #endif // 0
  // Init singleton
  gf::SingletonStorage<gf::Random> storageForRandom(gw::gRandom);

  gf::SingletonStorage<gf::AssetManager> storageForResourceManager(gw::gAssetManager);
  gw::gAssetManager().addSearchDir(GLOBAL_WAR_DATA_DIR);

  gf::SingletonStorage<gf::MessageManager> storageForMessageManager(gw::gMessageManager);

  gw::NetworkManagerServer networkManager(argv[1], argv[2]);

  // Wait for new connexions
  std::thread([&networkManager]() {
    for (;;) {
      networkManager.waitConnection();
    }
  }).detach();

  gw::Lobby lobby(networkManager);

  gf::Clock clock;
  for(;;) {
    lobby.processPackets();

    gf::Time time = clock.restart();
    lobby.update();

    if (time < gw::TickTime) {
      gf::sleep(gw::TickTime - time);
    }
  }

  return 0;

}
