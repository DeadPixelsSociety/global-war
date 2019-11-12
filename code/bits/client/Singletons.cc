#include "Singletons.h"

namespace gw {
  gf::Singleton<gf::ResourceManager> gResourceManager;
  gf::Singleton<gf::MessageManager> gMessageManager;
  gf::Singleton<NetworkManagerClient> gNetwork;
}
