#ifndef BITS_CLIENT_NETWORK_MANAGER_H
#define BITS_CLIENT_NETWORK_MANAGER_H

#include <gf/Queue.h>

#include "../common/Sockets.h"

namespace gw {
  class NetworkManagerClient {
  public:
    NetworkManagerClient(const char* hostname, const char* portLobby, const char* portGame);

    // gf::Id getPlayerID();

    bool receiveLobbyPacket(PacketLobbyClient &packet);

  private:
    SocketTcp m_lobbySocket;
    // SocketTcp m_gameSocket;

    gf::Queue<PacketLobbyClient> m_lobbyQueue;
  };
}

#endif // BITS_CLIENT_NETWORK_MANAGER_H
