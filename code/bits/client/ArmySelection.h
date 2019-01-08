#ifndef GW_ARMY_SELECTION_H
#define GW_ARMY_SELECTION_H

#include <gf/Event.h>
#include <gf/RenderTarget.h>
#include <gf/Vector.h>

#include "ClientArmy.h"
#include "ClientMap.h"

namespace gw {

  class ArmySelection {
  public:
    ArmySelection(ClientMap& map, ClientArmy& army, const gf::RenderTarget& target, const gf::View& view);

    void processEvent(const gf::Event& event);

  private:
    enum class State {
      WaitingRegiment,
      WaitingDestination,
      Done,
    };

    State m_state = State::WaitingRegiment;

    const ClientMap *m_map;
    const ClientArmy *m_army;
    const gf::RenderTarget *m_target;
    const gf::View *m_view;

    gf::Vector2i m_source;
    gf::Vector2i m_destination;

  };

}

#endif // GW_ARMY_SELECTION_H
