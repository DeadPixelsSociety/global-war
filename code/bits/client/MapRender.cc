#include "MapRender.h"

#include <gf/Sprite.h>
#include <gf/SpriteBatch.h>
#include <gf/RenderTarget.h>

#include "../common/Hexagon.h"

#include "Singletons.h"

namespace gw {

  MapRender::MapRender(GameState &gameState)
  : m_gameState(gameState)
  , m_texture(gResourceManager().getTexture("tileset.png"))
  {

  }

  void MapRender::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::SpriteBatch batch(target);
    batch.begin();

    for (gf::Vector2i pos : m_gameState.data.map.getPositionRange()) {
      gf::Sprite sprite(m_texture);

      MapData::TileType type = static_cast<MapData::TileType>(m_gameState.data.map.getTile(pos));

      switch (type) {
        case MapData::TileType::Sea:
          sprite.setTextureRect(gf::RectF({ 0.125f, 0.0f }, { 0.125f, 0.125f }));
          break;
        case MapData::TileType::Land:
          sprite.setTextureRect(gf::RectF({ 0.0f, 0.0f }, { 0.125f, 0.125f }));
          break;
        default:
          assert(false);
          break;
      }

      sprite.setScale(Hexagon::Size * gf::Sqrt3 / 120.0f);
      sprite.setPosition(Hexagon::positionToCoordinates(pos));
      sprite.setAnchor(gf::Anchor::Center);

      batch.draw(sprite, states);
    }

    batch.end();
  }

}
