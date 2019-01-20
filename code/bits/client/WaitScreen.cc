#include "WaitScreen.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Text.h>
#include <gf/StringUtils.h>

namespace gw {
  WaitScreen::WaitScreen(gf::Font &font)
  : m_font(font) {
  }

  void WaitScreen::update(gf::Time time) {
    m_elapsedTime += time;
  }

  void WaitScreen::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    gf::Text text("Waiting players...\n" + gf::niceNum(m_elapsedTime.asSeconds(), 1), m_font, coordinates.getRelativeCharacterSize(0.075f));
    text.setColor(gf::Color::Black);
    text.setParagraphWidth(coordinates.getRelativeSize({1.0f, 1.0f}).width);
    text.setAlignment(gf::Alignment::Center);
    text.setPosition(coordinates.getCenter());
    text.setAnchor(gf::Anchor::Center);

    text.draw(target, states);
  }
}
