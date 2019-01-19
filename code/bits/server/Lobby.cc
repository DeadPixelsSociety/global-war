#include "Lobby.h"

#include <thread>

#include <gf/Id.h>
#include <gf/Log.h>

using boost::asio::ip::tcp;
using namespace gf::literals;

namespace gw {
  Lobby::Lobby(gf::Random &random, std::uint16_t port)
  : m_random(random)
  , m_listener(port) {
    m_listener.handleNewConnection([this](SocketTcp clientSocket){
      this->addNewPlayer(std::move(clientSocket));
    });
  }

  void Lobby::addNewPlayer(SocketTcp socket) {
    // Generate a new ID
    gf::Id id = generateId();
    std::map<gf::Id, ThreadCom>::iterator it;

    // Create a new player
    {
      std::lock_guard<std::mutex> mutexLock(m_playerMutex);
      std::tie(it, std::ignore) = m_players.emplace(id, ThreadCom(std::move(socket), m_comQueue, id));
    }
    it->second.start();

    // Send to the client his ID
    Packet packet;
    packet.type = PacketType::NewPlayer;
    packet.newPlayer.playerID = id;
    it->second.sendPacket(packet);
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

    return number;
  }
}
