#include "ClientModel.h"

#include <cassert>

namespace gw {
  gf::Color4f ClientModel::getPlayerColor(gf::Id playerID) {
    if (allPlayerID[0] == playerID) {
      return gf::Color::Green;
    }
    if (allPlayerID[1] == playerID) {
      return gf::Color::Yellow;
    }
    if (allPlayerID[2] == playerID) {
      return gf::Color::Red;
    }

    assert(false); // Color not yet defined
    return gf::Color::White;
  }
}
