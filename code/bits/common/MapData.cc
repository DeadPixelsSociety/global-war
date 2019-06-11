#include "MapData.h"

#include <cassert>

#include <iostream>
#include <fstream>

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

#include "Hexagon.h"

namespace gw {

  namespace {
    constexpr float MinDistanceSpawnLocation = 5.0f * Hexagon::Size;
  }

  MapData::MapData(const gf::Path& path)
  : m_map({ Width, Height }, TileType::Sea)
  {
    std::ifstream file(path.string());
    int y = 0;

    for (std::string line; std::getline(file, line); ) {
      assert(line.size() == Width);
      assert(y < Height);

      for (int x = 0; x < Width; ++x) {
        switch (line[x]) {
          case '#':
            m_map({ x, y }) = TileType::Sea;
            std::cout << '#';
            break;
          case ' ':
            m_map({ x, y }) = TileType::Land;
            std::cout << ' ';
            break;
        }
      }

      std::cout << '\n';
      ++y;
    }

  }

  gf::Vector2i MapData::getTileCoordinate(gf::Vector2f screenCoordinate) const {
    return Hexagon::coordinatesToPosition(screenCoordinate);
  }

  gf::PositionRange<int> MapData::getPositionRange() const {
    return m_map.getPositionRange();
  }

  MapData::TileType MapData::getTile(gf::Vector2i position) const {
    return m_map(position);
  }

  std::vector<gf::Vector2i> MapData::generateInitialPositions(gf::Random &random, size_t nbPlayers) const {
    std::vector<gf::Vector2i> positions;

    do {
      // Choose random location
      auto position = random.computePosition(gf::RectI(0, 0, Width - 1, Height - 1));

      // Check if is not in sea
      if (m_map(position) == TileType::Sea) {
        continue;
      }

      // Check if at least three empty position next to origin
      std::size_t validNeighbors = getEmptyNeighborPositions(position).size();

      if (validNeighbors < 3) {
        continue;
      }

      // Check if far enough from the previous positions
      auto worldPosition = Hexagon::positionToCoordinates(position);
      bool validDistance = true;
      for (auto previousInitialPosition: positions) {
        auto worldPreviousPosition = Hexagon::positionToCoordinates(previousInitialPosition);

        if (gf::euclideanDistance(worldPosition, worldPreviousPosition) < MinDistanceSpawnLocation) {
          validDistance = false;
        }
      }

      if (!validDistance) {
        continue;
      }

      // Add to the positions
      positions.push_back(position);
    } while (positions.size() < nbPlayers);

    return positions;
  }

  std::vector<gf::Vector2i> MapData::getEmptyNeighborPositions(gf::Vector2i position) const {
    std::vector<gf::Vector2i> neighbors;

    for (auto neighbor: Hexagon::getNeighbors(position)) {
      if (m_map(neighbor) != TileType::Sea) {
        neighbors.push_back(neighbor);
      }
    }

    return neighbors;
  }

}
