#include "NetworkManagerClient.h"

namespace gw {

  NetworkManagerClient::NetworkManagerClient(const char* hostname, const char* portLobby, const char* portGame)
  : m_lobbySocket(hostname, portLobby)
  , m_gameSocket(hostname, portGame) {

    // Receive lobby packets
    std::thread([&](){
      PacketLobbyClient packet;
      while (m_lobbySocket.receive(packet)) {
        m_lobbyQueue.push(packet);
      }

      // TODO: handle disconnections
    }).detach();

    // Receive game packets
    std::thread([&](){
      PacketGameClient packet;
      while (m_gameSocket.receive(packet)) {
        m_gameQueue.push(packet);
      }

      // TODO: handle disconnections
    }).detach();

  }

  bool NetworkManagerClient::sendLobbyPacket(PacketLobbyServer &packet) {
    return m_lobbySocket.send(packet);
  }

  bool NetworkManagerClient::receiveLobbyPacket(PacketLobbyClient &packet) {
    return m_lobbyQueue.poll(packet);
  }

  bool NetworkManagerClient::sendGamePacket(PacketGameServer &packet) {
    return m_gameSocket.send(packet);
  }

  bool NetworkManagerClient::receiveGamePacket(PacketGameClient &packet) {
    return m_gameQueue.poll(packet);
  }

}
