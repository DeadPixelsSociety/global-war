#ifndef _LOCAL_SINGLETONS_H
#define _LOCAL_SINGLETONS_H

#include <gf/MessageManager.h>
#include <gf/ResourceManager.h>
#include <gf/Singleton.h>

namespace gw {
  extern gf::Singleton<gf::ResourceManager> gResourceManager;
  extern gf::Singleton<gf::MessageManager> gMessageManager;
}

#endif // _LOCAL_SINGLETONS_H
