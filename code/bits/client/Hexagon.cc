#include "Hexagon.h"

#include <algorithm>
#include <vector>

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

    gf::Vector2i coordinatesToPosition(gf::Vector2f coords) {
      gf::Vector2i position = coords / Step;

      std::vector<gf::Vector2i> candidates;

      candidates.push_back(position);

      if (position.y % 2 == 0) {
        candidates.push_back({ position.x - 1, position.y     });
        candidates.push_back({ position.x + 1, position.y     });
        candidates.push_back({ position.x - 1, position.y - 1 });
        candidates.push_back({ position.x    , position.y - 1 });
        candidates.push_back({ position.x - 1, position.y + 1 });
        candidates.push_back({ position.x    , position.y + 1 });
      } else {
        candidates.push_back({ position.x - 1, position.y     });
        candidates.push_back({ position.x + 1, position.y     });
        candidates.push_back({ position.x    , position.y - 1 });
        candidates.push_back({ position.x + 1, position.y - 1 });
        candidates.push_back({ position.x    , position.y + 1 });
        candidates.push_back({ position.x + 1, position.y + 1 });
      }

      return *std::min_element(candidates.begin(), candidates.end(), [coords](gf::Vector2i lhs, gf::Vector2i rhs) { return gf::squareDistance(coords, positionToCoordinates(lhs)) < gf::squareDistance(coords, positionToCoordinates(rhs)); });
    }

  }

}
