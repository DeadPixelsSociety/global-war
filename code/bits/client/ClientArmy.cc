#include "ClientArmy.h"

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Text.h>

#include "../common/Hexagon.h"

namespace gw {

  ClientArmy::ClientArmy(gf::ResourceManager& resources)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_playerID(InvalidPlayerID)
  {
  }

  const Regiment* ClientArmy::getRegiment(gf::Vector2i position) const {
    auto it = m_army.find(position);

    if (it == m_army.end()) {
      return nullptr;
    }

    return std::addressof(*it);
  }


  void ClientArmy::createRegiment(int count, gf::Vector2i position, gf::Id ownerID) {
    m_army.insert({ count, position, ownerID });
  }

  void ClientArmy::setPlayerID(gf::Id playerID) {
    m_playerID = playerID;
  }

  void ClientArmy::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (auto& regiment : m_army) {
      auto position = Hexagon::positionToCoordinates(regiment.position);

      gf::Color4f colorLight;
      if (regiment.ownerID == m_playerID) {
        colorLight = gf::Color::Yellow;
      }
      else {
        colorLight = gf::Color::Green;
      }
      gf::Color4f colorMedium = gf::Color::darker(colorLight);
      gf::Color4f colorDark = gf::Color::darker(colorMedium);

      gf::CircleShape shape(Hexagon::Size / 2);
      shape.setColor(colorLight);
      shape.setOutlineColor(colorDark);
      shape.setOutlineThickness(Hexagon::Size / 40);
      shape.setPosition(position);
      shape.setAnchor(gf::Anchor::Center);
      target.draw(shape, states);


      gf::Text text(std::to_string(regiment.count), m_font, Hexagon::Size / 2);
      text.setColor(colorMedium);
      text.setOutlineColor(colorDark);
      text.setOutlineThickness(Hexagon::Size / 40);
      text.setPosition(position);
      text.setAnchor(gf::Anchor::Center);
      target.draw(text, states);
    }
  }

}
