#include "ClientArmy.h"

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Hexagon.h"
namespace gw {

  ClientArmy::ClientArmy()
  {
    m_army.push_back({ 40, { 16, 16 } });
  }

  void ClientArmy::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (auto& regiment : m_army) {
      gf::CircleShape shape(Hexagon::Size / 2);
      shape.setColor(gf::Color::Yellow);
      shape.setPosition(Hexagon::positionToCoordinates(regiment.position));
      shape.setAnchor(gf::Anchor::Center);
      target.draw(shape, states);
    }
  }

}
