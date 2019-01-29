#include "ClientMap.h"

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "../common/Hexagon.h"

namespace gw {

  ClientMap::ClientMap(const gf::Path& path)
  : MapModel(path) {

  }

  void ClientMap::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (gf::Vector2i pos : m_map.getPositionRange()) {
      gf::CircleShape hex(Hexagon::Size, 6);

      TileType type = static_cast<TileType>(m_map(pos));

      switch (type) {
        case TileType::Sea:
          hex.setColor(gf::Color::Azure);
          break;
        case TileType::Land:
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
