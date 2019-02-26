#ifndef BITS_CLIENT_MODEL_H
#define BITS_CLIENT_MODEL_H

#include <vector>

#include <gf/Color.h>
#include <gf/Id.h>

namespace gw {
  struct ClientModel {
    // Players management
    gf::Id currentPlayerID;
    std::vector<gf::Id> allPlayerID;
    gf::Color4f getPlayerColor(gf::Id playerID);

  };
}

#endif // BITS_CLIENT_MODEL_H
