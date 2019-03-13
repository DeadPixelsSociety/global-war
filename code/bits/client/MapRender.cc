#include "MapRender.h"

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "../common/Hexagon.h"

namespace gw {

  MapRender::MapRender(GameState &gameState)
  : m_gameState(gameState) {

  }

  void MapRender::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (gf::Vector2i pos : m_gameState.data.map.getPositionRange()) {
      gf::CircleShape hex(Hexagon::Size, 6);

      MapData::TileType type = static_cast<MapData::TileType>(m_gameState.data.map.getTile(pos));

      switch (type) {
        case MapData::TileType::Sea:
          hex.setColor(gf::Color::Azure);
          break;
        case MapData::TileType::Land:
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
