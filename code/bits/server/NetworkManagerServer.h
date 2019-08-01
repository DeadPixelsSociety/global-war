#ifndef BITS_SERVER_NETWORK_MANAGER_H
#define BITS_SERVER_NETWORK_MANAGER_H

#include "../common/Packets.h"
#include "../common/Sockets.h"

namespace gw {

  class NetworkManagerServer {
  public:
    NetworkManagerServer(const char* portLobby, const char* portGame);

    void waitConnection();

  private:
    gf::Id generateId() const;

    void sendLobbyPacket(gf::Id playerID, PacketLobbyClient &packet);

  private:
    ListenerTcp m_lobbyListener;
    ListenerTcp m_gameListener;

    std::map<gf::Id, SocketTcp> m_lobbyClients;
    std::map<gf::Id, SocketTcp> m_gameClients;
  };

}

#endif // BITS_SERVER_NETWORK_MANAGER_H
