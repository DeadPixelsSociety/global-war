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

      // Create the lobby queue
      std::map<gf::Id, gf::Queue<PacketLobbyServer>>::iterator itLobbyQueue;

      std::tie(itLobbyQueue, inserted) = m_lobbyQueues.emplace(std::piecewise_construct,
        std::forward_as_tuple(playerID),
        std::forward_as_tuple());
      assert(inserted);

      // Launch the thread to receive the packets
      std::thread([&](){
        PacketLobbyServer packet;
        while (itLobbySocket->second.receive(packet)) {
          std::unique_lock<std::mutex> lock(m_mutexLobbyPackets);
          itLobbyQueue->second.push(packet);
          ++m_pendingLobbyPackets;

          m_conditionLockMutexLobby.notify_all();
        }
      }).detach();

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

  bool NetworkManagerServer::receiveLobbyPacket(gf::Id playerID, PacketLobbyServer &packet) {
    std::unique_lock<std::mutex> lock(m_mutexLobbyPackets);

    // Wait at least one packets
    while (m_pendingLobbyPackets < 1){
      m_conditionLockMutexLobby.wait(lock);
    }

    // Get the client queue, throw an exception if client not exists
    auto &clientQueue = m_lobbyQueues.at(playerID);

    // Get one packet
    if (clientQueue.poll(packet)) {
      --m_pendingLobbyPackets;
      return true;
    }

    return false;
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
