#ifndef BITS_SERVER_NETWORK_MANAGER_H
#define BITS_SERVER_NETWORK_MANAGER_H

#include <condition_variable>
#include <mutex>

#include <gf/Queue.h>

#include "../common/Packets.h"
#include "../common/Sockets.h"

namespace gw {

  class NetworkManagerServer {
  public:
    NetworkManagerServer(const char* portLobby, const char* portGame);

    void waitConnection();

    bool receiveLobbyPacket(gf::Id playerID, PacketLobbyServer &packet);

  private:
    gf::Id generateId() const;

    void sendLobbyPacket(gf::Id playerID, PacketLobbyClient &packet);

  private:

  private:
    ListenerTcp m_lobbyListener;
    ListenerTcp m_gameListener;

    std::map<gf::Id, SocketTcp> m_lobbyClients;
    std::map<gf::Id, SocketTcp> m_gameClients;

    std::map<gf::Id, gf::Queue<PacketLobbyServer>> m_lobbyQueues;
    std::map<gf::Id, gf::Queue<PacketGameServer>> m_gameQueues;

    std::mutex m_mutexLobbyPackets;
    std::condition_variable m_conditionLockMutexLobby;
    int m_pendingLobbyPackets;
  };

}

#endif // BITS_SERVER_NETWORK_MANAGER_H
