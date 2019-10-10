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

  bool NetworkManagerClient::receiveLobbyPacket(PacketLobbyClient &packet) {
    return m_lobbyQueue.poll(packet);
  }

  bool NetworkManagerClient::sendLobbyPacket(PacketLobbyServer &packet) {
    return m_lobbySocket.send(packet);
  }

}
