#include "MapModel.h"

#include <cassert>

#include <iostream>
#include <fstream>

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

#include "Hexagon.h"

namespace gw {

  MapModel::MapModel(const gf::Path& path)
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

  gf::Vector2i MapModel::getPosition(gf::Vector2f coords) const {
    return Hexagon::coordinatesToPosition(coords);
  }

}
