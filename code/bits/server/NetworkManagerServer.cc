#include "NetworkManagerServer.h"

#include <boost/asio.hpp>

#include <gf/Log.h>

#include "Messages.h"
#include "Singletons.h"
#include "Utils.h"


  /**
   * [X] Supprimer la map de LobbyQueue pour n'en mettre qu'une (le mutex est déjà dedans)
   * [X] L'attente des paquets du lobby se fera sur la queue unique
   * [ ] Faire deux fonctions de waitConnexion pour le lobby et le game
   * [X] Bien penser à passer les itérateurs par copie !
   * [ ] Pour gérer les deconnexions, faire le delete pour les deux map en dehors de la boucle de chaque thread (les iterateurs resteront valides mais pas si c'est lui même qui est détruit)
   * [ ] Lors de la connexion d'un game socket, vérifier que le PlayerID est présent dans la map de lobby
   * [ ]  si oui ok, on crée la socket jeu
   * [ ]  si non on casse le truc
   * [ ] Avant de lancer la session de jeu on vérifie que les deux sockets sont crées
   */

namespace gw {

  NetworkManagerServer::NetworkManagerServer(const char* portLobby, const char* portGame)
  : m_lobbyListener(std::atoi(portLobby))
  , m_gameListener(std::atoi(portGame))
  , m_pendingLobbyPackets(0) {

  }

  void NetworkManagerServer::waitConnection() {
    gf::Id playerID = generateId();

    m_lobbyListener.waitNewConnection([&](boost::asio::ip::tcp::socket socket) {
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

      gf::Log::info("New player into the lobby with ID #%lx\n", playerID);

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

    m_gameListener.waitNewConnection([&](boost::asio::ip::tcp::socket socket){
      // Create the game socket
      std::map<gf::Id, SocketTcp>::iterator itGameSocket;
      bool inserted;

      std::tie(itGameSocket, inserted) = m_gameClients.emplace(std::piecewise_construct,
        std::forward_as_tuple(playerID),
        std::forward_as_tuple(std::move(socket)));
      assert(inserted);

      // Create the game queue
      std::map<gf::Id, gf::Queue<PacketGameServer>>::iterator itGameQueue;

      std::tie(itGameQueue, inserted) = m_gameQueues.emplace(std::piecewise_construct,
        std::forward_as_tuple(playerID),
        std::forward_as_tuple());
      assert(inserted);

      // Launch the thread to receive the packets
      std::thread([&, itGameSocket, itGameQueue](){
        PacketGameServer packet;
        while (itGameSocket->second.receive(packet)) {
          itGameQueue->second.push(packet);
        }

        // TODO: handle disconnections
        m_gameClients.erase(itGameSocket);
        m_gameQueues.erase(itGameQueue);
      }).detach();

      gf::Log::info("New player into game with ID #%lx\n", playerID);
    });
  }

  bool NetworkManagerServer::receiveLobbyPackets(PacketLobbyServer &packet) {
    // Get one packet
    return m_lobbyQueue.poll(packet);
  }

  void NetworkManagerServer::sendLobbyPacket(PacketLobbyClient &packet) {
    sendLobbyPacket(packet.playerID, packet);
  }

  void NetworkManagerServer::sendLobbyPacket(gf::Id playerID, PacketLobbyClient &packet) {
    // Get the client socket, throw an exception if client not exists
    auto &clientSocket = m_lobbyClients.at(playerID);

    // Send the packet
    clientSocket.send(packet);
  }

  void NetworkManagerServer::sendGamePacket(gf::Id playerID, PacketGameClient &packet) {
    // Get the client socket, throw an exception if client not exists
    auto &clientSocket = m_gameClients.at(playerID);

    // Send the packet
    clientSocket.send(packet);
  }
}
