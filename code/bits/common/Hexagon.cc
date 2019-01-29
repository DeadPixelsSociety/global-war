#include "Hexagon.h"

#include <algorithm>
#include <vector>

#include <gf/VectorOps.h>

namespace gw {

  namespace {

    constexpr gf::Vector2i Neighbors[2][6] = {
      { { -1, 0 }, { 1, 0 }, { -1, -1 }, { 0, -1 }, { -1, 1 },            { 0, 1 }           },
      { { -1, 0 }, { 1, 0 },             { 0, -1 },            { 1, -1 }, { 0, 1 }, { 1, 1}  }
    };

  }


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

      for (auto pos : Neighbors[position.y % 2]) {
        candidates.push_back(position + pos);
      }

      return *std::min_element(candidates.begin(), candidates.end(), [coords](gf::Vector2i lhs, gf::Vector2i rhs) { return gf::squareDistance(coords, positionToCoordinates(lhs)) < gf::squareDistance(coords, positionToCoordinates(rhs)); });
    }

    bool areNeighbors(gf::Vector2i lhs, gf::Vector2i rhs) {
      for (auto pos : Neighbors[lhs.y % 2]) {
        if (lhs + pos == rhs) {
          return true;
        }
      }

      return false;
    }

  }

}
