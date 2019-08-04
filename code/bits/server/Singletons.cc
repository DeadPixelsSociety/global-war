#include "Singletons.h"

namespace gw {
  gf::Singleton<gf::AssetManager> gAssetManager;
  gf::Singleton<gf::Random> gRandom;
  gf::Singleton<gf::MessageManager> gMessageManager;
  // gf::Singleton<NetworkManagerServer> gNetworkManager; // @jube How to pass parameters to a singleton ?
}
