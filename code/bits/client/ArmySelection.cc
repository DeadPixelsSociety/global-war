#include "ArmySelection.h"

#include <cassert>

#include <gf/Log.h>

#include "../common/Hexagon.h"
#include "../common/MapData.h"

namespace gw {

  namespace {

    bool isPositionValid(gf::Vector2i position) {
      return 0 <= position.x && position.x < MapData::Width && 0 <= position.y && position.y < MapData::Height;
    }

  }

  ArmySelection::ArmySelection(GameState &gameState, const gf::RenderTarget& target, const gf::View& view)
  : m_target(&target)
  , m_view(&view)
  , m_gameState(gameState)
  {

  }

  void ArmySelection::processEvent(const gf::Event& event) {
    if (event.type == gf::EventType::MouseButtonPressed) {
      switch (m_state) {
        case State::WaitingRegiment: {
          gf::Vector2i position = m_gameState.data.map.getTileCoordinate(m_target->mapPixelToCoords(event.mouseButton.coords, *m_view));

          if (isPositionValid(position)) {
            auto regiment = m_gameState.data.getRegiment(position);

            if (regiment != nullptr && regiment->ownerID == m_gameState.currentPlayerID) {
              m_source = position;
              gf::Log::info("source: %i x %i\n", position.x, position.y);
              m_state = State::WaitingDestination;
            }
          }

          break;
        }

        case State::WaitingDestination: {
          gf::Vector2i position = m_gameState.data.map.getTileCoordinate(m_target->mapPixelToCoords(event.mouseButton.coords, *m_view));
          gf::Log::info("possible destination: %i x %i\n", position.x, position.y);

          if (m_gameState.data.isValidMove(m_source, position)) {
            m_destination = position;
            gf::Log::info("destination: %i x %i\n", position.x, position.y);
            m_state = State::WaitingRegiment;

            // Send move to server
            Packet packet;
            packet.type = PacketType::MoveRegiment;
            packet.moveRegiment.playerID = m_gameState.currentPlayerID;
            packet.moveRegiment.regimentOrigin = m_source;
            packet.moveRegiment.regimentDestination = m_destination;

            if (!m_gameState.threadCom.sendPacket(packet)) {
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
