#include "NetworkManagerServer.h"

#include <boost/asio.hpp>

#include <gf/Log.h>

#include "Singletons.h"

namespace gw {

  NetworkManagerServer::NetworkManagerServer(const char* portLobby, const char* portGame)
  : m_lobbyListener(std::atoi(portLobby))
  , m_gameListener(std::atoi(portGame)) {
    m_lobbyListener.waitNewConnection([&](boost::asio::ip::tcp::socket socket){

      gf::Id playerID = generateId();

      std::map<gf::Id, SocketTcp>::iterator it;
      bool inserted;

      std::tie(it, inserted) = m_lobbyClients.emplace(std::piecewise_construct,
                              std::forward_as_tuple(playerID),
                              std::forward_as_tuple(std::move(socket)));
      assert(inserted);

      gf::Log::debug("New client: %lx\n", playerID);

      // Send the ID to the player
      PacketLobbyClient packet;
      packet.type = PacketLobbyClientType::CreatePlayer;
      packet.createPlayer.playerID = playerID;

      it->second.send(packet);
    });
  }

  gf::Id NetworkManagerServer::generateId() const {
    // Get the ID
    uint64_t min = std::numeric_limits<uint64_t>::min();
    uint64_t max = std::numeric_limits<uint64_t>::max();
    uint64_t number = gRandom().computeUniformInteger(min, max);

    return number;
  }

  void NetworkManagerServer::sendLobbyPacket(gf::Id playerID, PacketLobbyClient &packet) {
    // Get the client socket, throw an exception if client not exists
    auto &clientSocket = m_lobbyClients.at(playerID);

    // Send the packet
    clientSocket.send(packet);
  }
}
