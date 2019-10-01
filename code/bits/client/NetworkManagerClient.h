#ifndef BITS_CLIENT_NETWORK_MANAGER_H
#define BITS_CLIENT_NETWORK_MANAGER_H

#include "../common/Sockets.h"

namespace gw {
  class NetworkManagerClient {
  public:
    NetworkManagerClient(const char* hostname, const char* portLobby, const char* portGame);

    gf::Id getPlayerID();

  private:
    SocketTcp m_lobbySocket;
    // SocketTcp m_gameSocket;
  };
}

#endif // BITS_CLIENT_NETWORK_MANAGER_H
