#include "ClientModel.h"

#include <cassert>

#include <gf/Log.h>

namespace gw {
  ClientModel::ClientModel(char *hostname, char* port)
  : threadCom(hostname, port, comQueue) {
    // Retreive the player ID
    Packet packet;
    assert(threadCom.receivePacket(packet));
    assert(packet.type == PacketType::NewPlayer);

    currentPlayerID = packet.newPlayer.playerID;
    gf::Log::info("Player ID: %lx\n", currentPlayerID);
  }

  void ClientModel::quickMacth() {
    Packet packet;
    packet.type = PacketType::QuickMatch;
    packet.quickMatch.playerID = currentPlayerID;

    assert(threadCom.sendPacket(packet));
  }

  gf::Color4f ClientModel::getPlayerColor() {
    return getPlayerColor(currentPlayerID);
  }

  gf::Color4f ClientModel::getPlayerColor(gf::Id playerID) {
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
