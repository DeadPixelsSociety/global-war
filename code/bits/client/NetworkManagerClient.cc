#include "NetworkManagerClient.h"

namespace gw {

  NetworkManagerClient::NetworkManagerClient(const char* hostname, const char* portLobby, const char* portGame)
  : m_lobbySocket(hostname, portLobby) {

    // Receive lobby packets
    std::thread([&](){
      PacketLobbyClient packet;
      while (m_lobbySocket.receive(packet)) {
        m_lobbyQueue.push(packet);
      }

      // TODO: handle disconnections
    }).detach();

  }

  // gf::Id NetworkManagerClient::getPlayerID() {
  //   gw::PacketLobbyClient packet;
  //   m_lobbySocket.receive(packet);
  //
  //   assert(packet.type == gw::PacketLobbyClientType::CreatePlayer);
  //   assert(packet.playerID == packet.createPlayer.playerID);
  //
  //   return packet.playerID;
  // }

  bool NetworkManagerClient::receiveLobbyPacket(PacketLobbyClient &packet) {
    return m_lobbyQueue.poll(packet);
  }

}
