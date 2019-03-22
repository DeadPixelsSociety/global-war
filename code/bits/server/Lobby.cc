#include "Lobby.h"

#include <thread>

#include <gf/Id.h>
#include <gf/Log.h>

using boost::asio::ip::tcp;
using namespace gf::literals;

namespace gw {
  static constexpr int GameLimitPlayer = 3;

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
    std::map<gf::Id, Player>::iterator it;

    // Create a new player
    {
      std::lock_guard<std::mutex> mutexLock(m_playerMutex);
      std::tie(it, std::ignore) = m_players.emplace(id, Player(std::move(socket), m_comQueue, id));
    }
    it->second.initialize();

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
    case PacketType::NewPlayer:
      gf::Log::info("Player ID: %lx\n", packet.newPlayer.playerID);
      break;

    case PacketType::QuickMatch:
    {
      gf::Id playerID = packet.quickMatch.playerID;
      gf::Log::info("Player ID: %lx\n", playerID);
      m_players.at(playerID).waitGame();
      createNewGame();
      gf::Log::info("Available players: %lu\n", m_players.size());
      break;
    }

    case PacketType::JoinGame:
      gf::Log::info("Game ID: %lx\n", packet.joinGame.gameID);
      break;

    case PacketType::CreateRegiment:
    case PacketType::MoveRegiment:
    case PacketType::MoveUnit:
      assert(false);
      break;
    }
  }

  void Lobby::createNewGame() {
    if (std::count_if(m_players.begin(), m_players.end(), [](const auto &entry) {
      return entry.second.getState() == Player::State::WaitingMatch;
    }) >= GameLimitPlayer) {
      // Start game
      gf::Id gameID = generateId();

      // Create new Session game
      bool inserted;
      decltype(m_games)::iterator it;

      std::tie(it, inserted) = m_games.emplace(std::piecewise_construct,
        std::forward_as_tuple(gameID),
        std::forward_as_tuple(gameID, m_players));

      assert(inserted);

      // Launch game
      it->second.launchGame();
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
