#ifndef BITS_SERVER_GAME_MODEL_H
#define BITS_SERVER_GAME_MODEL_H

#include <gf/Model.h>

#include "../common/RegimentContainer.h"

namespace gw {

  struct GameModel: public gf::Model {
    GameModel();

    void update(gf::Time time) override;

    RegimentContainer regiments;
  };
}

#endif // BITS_SERVER_GAME_MODEL_H
