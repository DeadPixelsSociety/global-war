#include "MapData.h"

#include <cassert>

#include <iostream>
#include <fstream>

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

#include "Hexagon.h"

namespace gw {

  MapData::MapData(const gf::Path& path)
  : m_map({ Width, Height }, static_cast<int>(TileType::Sea))
  {
    std::ifstream file(path.string());
    int y = 0;

    for (std::string line; std::getline(file, line); ) {
      assert(line.size() == Width);
      assert(y < Height);

      for (int x = 0; x < Width; ++x) {
        switch (line[x]) {
          case '#':
            m_map({ x, y }) = static_cast<int>(TileType::Sea);
            std::cout << '#';
            break;
          case ' ':
            m_map({ x, y }) = static_cast<int>(TileType::Land);
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

  int MapData::getTile(gf::Vector2i position) const {
    return m_map(position);
  }

}
