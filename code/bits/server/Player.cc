#include "Player.h"

namespace gw {
  Player::Player(SocketTcp socket, gf::Queue<Packet> &queue, gf::Id playerId)
  : m_com(std::move(socket), queue)
  , m_state(State::Idle) {
  }

  Player::State Player::getState() const {
    return m_state;
  }

  void Player::waitGame() {
    m_state = State::WaitingMatch;
  }

  void Player::sendPacket(Packet &packet) {
    m_com.sendPacket(packet);
  }

  void Player::initialize() {
    m_com.start();
  }

  void Player::updateQueue(gf::Queue<Packet> *queue) {
    m_com.setQueue(queue);
  }
}
