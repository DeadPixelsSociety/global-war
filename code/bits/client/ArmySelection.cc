#include "ArmySelection.h"

#include <cassert>

#include <gf/Log.h>

namespace gw {

  ArmySelection::ArmySelection(ClientMap& map, ClientArmy& army, const gf::RenderTarget& target, const gf::View& view)
  : m_map(&map)
  , m_army(&army)
  , m_target(&target)
  , m_view(&view)
  {

  }

  void ArmySelection::processEvent(const gf::Event& event) {
    if (event.type == gf::EventType::MouseButtonPressed) {
      switch (m_state) {
        case State::None: {
          gf::Vector2i position = m_map->getPosition(m_target->mapPixelToCoords(event.mouseButton.coords, *m_view));

          if (m_army->hasRegiment(position)) {
            gf::Log::info("regiment: %i x %i\n", position.x, position.y);
          }

          break;
        }

        default:
          assert(false);
      }
    }
  }

}
