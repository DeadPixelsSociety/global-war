#include "GameState.h"

#include <cassert>

#include <gf/Color.h>
#include <gf/Log.h>

#include "Singletons.h"

namespace gw {
  GameState::GameState(char *hostname, char* port)
  : threadCom(hostname, port, comQueue)
  , data(gResourceManager().getAbsolutePath("map.txt")) {
    // Retreive the player ID
    Packet packet;

    if (!threadCom.receivePacket(packet)) {
      assert(false);
    }

    assert(packet.type == PacketType::NewPlayer);

    currentPlayerID = packet.newPlayer.playerID;
    gf::Log::info("Player ID: %lx\n", currentPlayerID);
  }

  void GameState::update(gf::Time time) {
    data.cleanUpRegiments();
  }

  void GameState::quickMatch() {
    Packet packet;
    packet.type = PacketType::QuickMatch;
    packet.quickMatch.playerID = currentPlayerID;

    if (!threadCom.sendPacket(packet)) {
      assert(false);
    }
  }

  gf::Color4f GameState::getPlayerColor() {
    return getPlayerColor(currentPlayerID);
  }

  gf::Color4f GameState::getPlayerColor(gf::Id playerID) {
    if (allPlayerID[0] == playerID) {
      return gf::Color::Green;
    }
    if (allPlayerID[1] == playerID) {
      return gf::Color::Yellow;
    }
    if (allPlayerID[2] == playerID) {
      return gf::Color::Red;
    }

    assert(false); // Color not yet defined
    return gf::Color::White;
  }
}
