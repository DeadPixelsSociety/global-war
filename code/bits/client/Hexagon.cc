#include "Hexagon.h"

#include <gf/VectorOps.h>

namespace gw {
//     constexpr gf::Vector2i posToHex(gf::Vector2i pos) {
//       return { pos.x * 2 + pos.y % 2, pos.y };
//     }

  namespace Hexagon {

    gf::Vector2f positionToCoordinates(gf::Vector2i position) {
      gf::Vector2f offset = Step / 2;

      if (position.y % 2 == 1) {
        offset.x += Step.width / 2;
      }

      return position * Step + offset;
    }

  }

}
