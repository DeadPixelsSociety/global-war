#ifndef GW_HEXAGON_H
#define GW_HEXAGON_H

#include <gf/Vector.h>
#include <gf/Math.h>

namespace gw {

  namespace Hexagon {
    static constexpr float Size = 64.0f;
    static constexpr gf::Vector2f Step(Size * gf::Sqrt3, 2 * Size * 3 / 4);

    gf::Vector2f positionToCoordinates(gf::Vector2i position);
  }

}

#endif // GW_HEXAGON_H

