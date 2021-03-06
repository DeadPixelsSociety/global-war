#include "Player.h"

#include <thread>

namespace gw {
  Player::Player(SocketTcp socket, gf::Queue<Packet> &queue, gf::Id playerID)
  : m_com(std::move(socket), queue)
  , m_state(State::Idle)
  , m_playerID(playerID) {
  }

  Player::State Player::getState() const {
    return m_state;
  }

  gf::Id Player::getID() const {
    return m_playerID;
  }

  void Player::waitGame() {
    m_state = State::WaitingMatch;
  }

  void Player::plays() {
    m_state = State::Playing;
  }

  void Player::sendPacket(Packet &packet) {
    bool ok = m_com.sendPacket(packet);
    assert(ok);
    if (!ok)
      throw std::runtime_error("!ok");
  }

  void Player::receivePacket(Packet &packet) {
    bool ok = m_com.receivePacket(packet);
    assert(ok);
  }

  void Player::initialize() {
    std::thread([this]() {
      m_com.start();
    }).detach();
  }

  void Player::updateQueue(gf::Queue<Packet> *queue) {
    m_com.setQueue(queue);
  }
}
