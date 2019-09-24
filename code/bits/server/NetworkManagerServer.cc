#include "NetworkManagerServer.h"

#include <boost/asio.hpp>

#include <gf/Log.h>

#include "Messages.h"
#include "Singletons.h"

namespace gw {

  NetworkManagerServer::NetworkManagerServer(const char* portLobby, const char* portGame)
  : m_lobbyListener(std::atoi(portLobby))
  , m_gameListener(std::atoi(portGame))
  , m_pendingLobbyPackets(0) {

  }

  void NetworkManagerServer::waitConnection() {
    m_lobbyListener.waitNewConnection([&](boost::asio::ip::tcp::socket socket){

      gf::Id playerID = generateId();

      // Create the lobby socket
      std::map<gf::Id, SocketTcp>::iterator itLobbySocket;
      bool inserted;

      std::tie(itLobbySocket, inserted) = m_lobbyClients.emplace(std::piecewise_construct,
        std::forward_as_tuple(playerID),
        std::forward_as_tuple(std::move(socket)));
      assert(inserted);

      // Launch the thread to receive the packets
      std::thread([&,itLobbySocket](){
        PacketLobbyServer packet;
        while (itLobbySocket->second.receive(packet)) {
          m_lobbyQueue.push(packet);
        }

        // TODO: handle disconnections
        m_lobbyClients.erase(itLobbySocket);
      }).detach();

      /**
       * Supprimer la map de LobbyQueue pour n'en mettre qu'une (le mutex est déjà dedans)
       * L'attente des paquets du lobby se fera sur la queue unique
       * Faire deux fonctions de waitConnexion pour le lobby et le game
       * Bien penser à passer les itérateurs par copie !
       * Pour gérer les deconnexions, faire le delete en dehors de la boucle de chaque thread (les iterateurs resteront valides)
       * Lors de la connexion d'un game socket, vérifier le PlayerID
       */

      // // Create the game socket
      // std::map<gf::Id, SocketTcp>::iterator itGameSocket;
      //
      // std::tie(itGameSocket, inserted) = m_lobbyClients.emplace(std::piecewise_construct,
      //   std::forward_as_tuple(playerID),
      //   std::forward_as_tuple(std::move(socket)));
      // assert(inserted);
      //
      // // Create the game queue
      // std::map<gf::Id, gf::Queue<PacketGameServer>>::iterator itGameQueue;
      //
      // std::tie(itGameQueue, inserted) = m_gameQueues.emplace(std::piecewise_construct,
      //   std::forward_as_tuple(playerID),
      //   std::forward_as_tuple());
      // assert(inserted);
      //
      // // Launch the thread to receive the packets
      // std::thread([&](){
      //   for (;;) {
      //     PacketLobbyServer packet;
      //
      //     itLobbySocket->second.receive(packet);
      //     itLobbyQueue->second.push(packet);
      //   }
      // }).detach();

      gf::Log::debug("New client: %lx\n", playerID);

      // Send the ID to the player
      PacketLobbyClient packet;
      packet.type = PacketLobbyClientType::CreatePlayer;
      packet.playerID = playerID;
      packet.createPlayer.playerID = playerID;

      itLobbySocket->second.send(packet);

      // Send the message
      PlayerInLobby playerInLobby;
      playerInLobby.playerID = playerID;
      gMessageManager().sendMessage(&playerInLobby);
    });
  }

  bool NetworkManagerServer::receiveLobbyPackets(PacketLobbyServer &packet) {
    // Get one packet
    return m_lobbyQueue.poll(packet);
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
