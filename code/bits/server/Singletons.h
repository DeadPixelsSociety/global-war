#ifndef _LOCAL_SINGLETONS_H
#define _LOCAL_SINGLETONS_H

#include <gf/AssetManager.h>
#include <gf/Random.h>
#include <gf/Singleton.h>

namespace gw {
  extern gf::Singleton<gf::AssetManager> gAssetManager;
  extern gf::Singleton<gf::Random> gRandom;
}

#endif // _LOCAL_SINGLETONS_H
