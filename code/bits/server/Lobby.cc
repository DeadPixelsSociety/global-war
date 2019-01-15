#include "Lobby.h"

#include <thread>

#include <gf/Id.h>
#include <gf/Log.h>

using boost::asio::ip::tcp;
using namespace gf::literals;

namespace gw {
  Lobby::Lobby(gf::Random &random, std::uint16_t port)
  : m_random(random)
  , m_port(port){
    // Thread to automaticaly accept all clients
    std::thread(&Lobby::waitNewPlayers, this).detach();
  }

  void Lobby::waitNewPlayers() {
    boost::asio::io_service ioService;
    tcp::acceptor a(ioService, tcp::endpoint(tcp::v4(), m_port));

    for (;;) {
      tcp::socket sock(ioService);
      a.accept(sock);

      // Generate a new ID
      gf::Id id = generateId();

      // Create a new player
      {
        std::lock_guard<std::mutex> mutexLock(m_playerMutex);
        m_players.emplace_back(std::move(sock), m_comQueue, id);
      }

      // Send to the client his ID
      Packet packet;
      packet.type = PacketType::NewPlayer;
      packet.newPlayer.playerID = id;
      m_players.back().sendPacket(packet);
    }
  }

  void Lobby::processPacket() {
    Packet packet;
    m_comQueue.wait(packet);

    switch (packet.type) {
    case PacketType::Ping:
      gf::Log::info("Ping: %d\n", packet.ping.sequence);
      break;

    case PacketType::NewPlayer:
      gf::Log::info("Player ID: %lx\n", packet.newPlayer.playerID);
      break;

    case PacketType::QuickMatch:
      gf::Log::info("Player ID: %lx\n", packet.quickMatch.playerID);
      break;
    }
  }


  gf::Id Lobby::generateId() const {
    // Get the ID
    uint64_t min = std::numeric_limits<uint64_t>::min();
    uint64_t max = std::numeric_limits<uint64_t>::max();
    uint64_t number = m_random.computeUniformInteger(min, max);

    // Convert the uint64 to hex string
    std::stringstream stream;
    stream << std::hex << number;
    std::string result(stream.str());

    // Return the hash
    return gf::hash(result);
  }
}
