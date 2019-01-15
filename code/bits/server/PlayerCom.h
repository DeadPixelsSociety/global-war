#ifndef BITS_SERVER_PLAYER_COM
#define BITS_SERVER_PLAYER_COM

#include <boost/asio.hpp>

#include <gf/Id.h>
#include <gf/Queue.h>

#include "../common/Packet.h"

namespace gw {
  class PlayerCom {
  public:
    PlayerCom(boost::asio::ip::tcp::socket sock, gf::Queue<Packet> &queue, gf::Id playerId);

    void sendPacket(Packet &packet);

  private:
    void receivePackets();

  private:
    boost::asio::ip::tcp::socket m_sock;
    gf::Queue<Packet>* m_queue;
    const gf::Id m_playerId;
  };
}

#endif // BITS_SERVER_PLAYER_COM
