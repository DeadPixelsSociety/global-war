#include "RegimentRender.h"

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Text.h>

#include "../common/Hexagon.h"

#include "Singletons.h"

namespace gw {

  RegimentRender::RegimentRender(GameState &gameState)
  : m_font(gResourceManager().getFont("DejaVuSans.ttf"))
  , m_gameState(gameState)
  {
  }

  void RegimentRender::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (auto& regiment : m_gameState.data.regiments) {
      auto position = Hexagon::positionToCoordinates(regiment.position);

      gf::Color4f colorLight = m_gameState.getPlayerColor(regiment.ownerID);
      gf::Color4f colorMedium = gf::Color::darker(colorLight);
      gf::Color4f colorDark = gf::Color::darker(colorMedium);

      auto draw = [&](gf::Shape& shape) {
        shape.setColor(colorLight);
        shape.setOutlineColor(colorDark);
        shape.setOutlineThickness(Hexagon::Size / 40);
        shape.setPosition(position);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape, states);
      };

      switch (regiment.division) {
        case Division::Horseman:
        {
          gf::StarShape shape(Hexagon::Size * 0.35f, Hexagon::Size * 0.5f, 8);
          draw(shape);
          break;
        }

        case Division::Swordsman:
        {
          gf::RectangleShape shape({Hexagon::Size, Hexagon::Size});
          draw(shape);
          break;
        }

        case Division::Lancer:
        {
          gf::CircleShape shape(Hexagon::Size * 0.5f);
          draw(shape);
          break;
        }
      }

      gf::Text text(std::to_string(regiment.count), m_font, Hexagon::Size * 0.5f);
      text.setColor(colorMedium);
      text.setOutlineColor(colorDark);
      text.setOutlineThickness(Hexagon::Size / 40);
      text.setPosition(position);
      text.setAnchor(gf::Anchor::Center);
      target.draw(text, states);
    }
  }

}
