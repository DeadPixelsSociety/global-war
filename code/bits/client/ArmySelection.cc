#include "ArmySelection.h"

#include <cassert>

#include <gf/Log.h>

#include "../common/Hexagon.h"

namespace gw {

  namespace {

    bool isPositionValid(gf::Vector2i position) {
      return 0 <= position.x && position.x < ClientMap::Width && 0 <= position.y && position.y < ClientMap::Height;
    }

  }

  ArmySelection::ArmySelection(ClientMap& map, ClientArmy& army, ClientModel& clientModel, const gf::RenderTarget& target, const gf::View& view)
  : m_map(&map)
  , m_army(&army)
  , m_clientModel(clientModel)
  , m_target(&target)
  , m_view(&view)
  {

  }

  void ArmySelection::processEvent(const gf::Event& event) {
    if (event.type == gf::EventType::MouseButtonPressed) {
      switch (m_state) {
        case State::WaitingRegiment: {
          gf::Vector2i position = m_map->getPosition(m_target->mapPixelToCoords(event.mouseButton.coords, *m_view));

          if (isPositionValid(position)) {
            auto regiment = m_army->getRegiment(position);

            if (regiment != nullptr && regiment->ownerID == m_clientModel.currentPlayerID) {
              m_source = position;
              gf::Log::info("source: %i x %i\n", position.x, position.y);
              m_state = State::WaitingDestination;
            }
          }

          break;
        }

        case State::WaitingDestination: {
          gf::Vector2i position = m_map->getPosition(m_target->mapPixelToCoords(event.mouseButton.coords, *m_view));
          gf::Log::info("possible destination: %i x %i\n", position.x, position.y);

          if (isPositionValid(position) && Hexagon::areNeighbors(m_source, position)) {
            m_destination = position;
            gf::Log::info("destination: %i x %i\n", position.x, position.y);
            m_state = State::WaitingRegiment;

            // Send move to server
            Packet packet;
            packet.type = PacketType::MoveRegiment;
            packet.moveRegiment.playerID = m_clientModel.currentPlayerID;
            packet.moveRegiment.regimentOrigin = m_source;
            packet.moveRegiment.regimentDestination = m_destination;

            if (!m_clientModel.threadCom.sendPacket(packet)) {
              std::abort();
            }
          }

          break;
        }

        case State::Done:
          break;
      }
    }
  }

}
