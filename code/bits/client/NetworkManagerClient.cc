#include "NetworkManagerClient.h"

namespace gw {

  NetworkManagerClient::NetworkManagerClient(const char* hostname, const char* portLobby, const char* portGame)
  : m_lobbySocket(hostname, portLobby) {

  }

  gf::Id NetworkManagerClient::getPlayerID() {
    gw::PacketLobbyClient packet;
    m_lobbySocket.receive(packet);

    assert(packet.type == gw::PacketLobbyClientType::CreatePlayer);
    assert(packet.playerID == packet.createPlayer.playerID);

    return packet.playerID;
  }

}
