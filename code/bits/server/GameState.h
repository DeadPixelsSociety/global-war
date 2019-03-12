#ifndef BITS_SERVER_GAME_STATE_H
#define BITS_SERVER_GAME_STATE_H

#include <gf/Model.h>

#include "../common/Data.h"

namespace gw {

  struct GameState: public gf::Model {
    void update(gf::Time time) override;

    Data data;
  };

}

#endif // BITS_SERVER_GAME_STATE_H
