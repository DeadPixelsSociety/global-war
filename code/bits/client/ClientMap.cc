#include "ClientMap.h"

#include <cassert>

#include <iostream>
#include <fstream>

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

#include "Hexagon.h"

namespace gw {

  namespace {

    static constexpr int Sea = 0;
    static constexpr int Land = 1;

  }

  ClientMap::ClientMap(const gf::Path& path)
  : m_map({ Width, Height }, Sea)
  {
    std::ifstream file(path.string());
    int y = 0;

    for (std::string line; std::getline(file, line); ) {
      assert(line.size() == Width);
      assert(y < Height);

      for (int x = 0; x < Width; ++x) {
        switch (line[x]) {
          case '#':
            m_map({ x, y }) = Sea;
            std::cout << '#';
            break;
          case ' ':
            m_map({ x, y }) = Land;
            std::cout << ' ';
            break;
        }
      }

      std::cout << '\n';
      ++y;
    }

  }

  void ClientMap::render(gf::RenderTarget& target, const gf::RenderStates& states) {


    for (gf::Vector2i pos : m_map.getPositionRange()) {
      gf::CircleShape hex(Hexagon::Size, 6);

      switch (m_map(pos)) {
        case Sea:
          hex.setColor(gf::Color::Azure);
          break;
        case Land:
          hex.setColor(gf::Color::Chartreuse);
          break;
        default:
          assert(false);
          break;
      }

      hex.setOutlineColor(gf::Color::Black);
      hex.setOutlineThickness(3.0f);
      hex.setPosition(Hexagon::positionToCoordinates(pos));
      hex.setAnchor(gf::Anchor::Center);

      target.draw(hex, states);
    }
  }

}

