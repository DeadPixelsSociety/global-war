#include "Lobby.h"

#include <iostream>
#include <thread>

using boost::asio::ip::tcp;

namespace gw {
  Lobby::Lobby(std::uint16_t port)
  : m_port(port){
    // Thread to automaticaly accept all clients
    std::thread(&Lobby::waitNewPlayers, this).detach();
  }

  void Lobby::waitNewPlayers() {
    boost::asio::io_service ioService;
    tcp::acceptor a(ioService, tcp::endpoint(tcp::v4(), m_port));

    for (;;) {
      tcp::socket sock(ioService);
      a.accept(sock);

      // Create a new player
      {
        std::lock_guard<std::mutex> mutexLock(m_playerMutex);
        m_players.emplace_back(std::move(sock), m_comQueue);
      }
    }
  }

  void Lobby::processPacket() {
    Packet packet;
    m_comQueue.wait(packet);

    switch (packet.type) {
    case PacketType::Ping:
      std::cout << "Ping: " << packet.ping.sequence << '\n';
      break;

    case PacketType::QuickMatch:
      std::cout << "Player ID: " << packet.quickMatch.playerID << '\n';
      break;
    }
  }
}
