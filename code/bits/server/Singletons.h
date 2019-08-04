#ifndef _LOCAL_SINGLETONS_H
#define _LOCAL_SINGLETONS_H

#include <gf/AssetManager.h>
#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/Singleton.h>

#include "NetworkManagerServer.h"

namespace gw {
  extern gf::Singleton<gf::AssetManager> gAssetManager;
  extern gf::Singleton<gf::Random> gRandom;
  extern gf::Singleton<gf::MessageManager> gMessageManager;
  // extern gf::Singleton<NetworkManagerServer> gNetworkManager;
}

#endif // _LOCAL_SINGLETONS_H
