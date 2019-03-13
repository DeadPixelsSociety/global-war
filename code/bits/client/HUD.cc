#include "HUD.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>

namespace gw {
  HUD::HUD(GameState &gameState) :
  m_gameState(gameState) {

  }

  void HUD::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    float outlineThickness = coordinates.getRelativeSize({0.01f, 0.01f}).width;
    auto rectangleSize = target.getSize() - 2.0f * outlineThickness;
    gf::RectangleShape rectangle(rectangleSize);
    rectangle.setColor(gf::Color::Opaque(0.0f));
    rectangle.setPosition({outlineThickness, outlineThickness});
    rectangle.setOutlineColor(m_gameState.getPlayerColor());
    rectangle.setOutlineThickness(outlineThickness);
    target.draw(rectangle, states);
  }
}
