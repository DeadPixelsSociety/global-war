#ifndef BITS_SERVER_PLAYER_COM
#define BITS_SERVER_PLAYER_COM

#include <boost/asio.hpp>

#include <gf/Queue.h>

#include "../common/Packet.h"

namespace gw {
  class PlayerCom {
  public:
    PlayerCom(boost::asio::ip::tcp::socket sock, gf::Queue<Packet> &queue);

  private:
    void receivePackets();

  private:
    boost::asio::ip::tcp::socket m_sock;
    gf::Queue<Packet>* m_queue;
  };
}

#endif // BITS_SERVER_PLAYER_COM
