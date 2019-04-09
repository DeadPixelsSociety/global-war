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

      int corner = 0;
      switch (regiment.division) {
      case Division::Horseman:
        corner = 3;
        break;

      case Division::Swordsman:
        corner = 30;
        break;

      case Division::Lancer:
        corner = 4;
        break;
      }

      gf::CircleShape shape(Hexagon::Size * 0.75f, corner);
      shape.setColor(colorLight);
      shape.setOutlineColor(colorDark);
      shape.setOutlineThickness(Hexagon::Size / 40);
      shape.setPosition(position);
      shape.setAnchor(gf::Anchor::Center);
      target.draw(shape, states);

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
