#include "ClientArmy.h"

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Text.h>

#include "../common/Hexagon.h"

namespace gw {

  ClientArmy::ClientArmy(gf::ResourceManager& resources, ClientModel &clientModel)
  : m_font(resources.getFont("DejaVuSans.ttf"))
  , m_clientModel(clientModel)
  {
  }

  const Regiment* ClientArmy::getRegiment(gf::Vector2i position) const {
    return m_regiments[position];
  }


  void ClientArmy::createRegiment(int count, gf::Vector2i position, gf::Id ownerID) {
    m_regiments.army.insert({ ownerID, count, position });
  }

  void ClientArmy::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (auto& regiment : m_regiments.army) {
      auto position = Hexagon::positionToCoordinates(regiment.position);

      gf::Color4f colorLight = m_clientModel.getPlayerColor(regiment.ownerID);
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
