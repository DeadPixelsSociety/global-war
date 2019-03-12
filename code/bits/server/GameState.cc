#include "GameState.h"

#include <gf/Log.h>

namespace gw {
  void GameState::update(gf::Time time) {
    // DEBUG: Just print the regiments location
    for (auto &regiment: data.regiments) {
      gf::Log::debug("regiment at {%d, %d} with count: %d and owner: %lx\n", regiment.position.x, regiment.position.y, regiment.count, regiment.ownerID);
    }
  }
}
