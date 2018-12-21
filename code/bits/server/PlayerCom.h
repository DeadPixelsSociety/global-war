#ifndef BITS_SERVER_PLAYER_COM
#define BITS_SERVER_PLAYER_COM

#include <boost/asio.hpp>

#include "../common/Packet.h"
#include "../common/Queue.h"

namespace gw {
  class PlayerCom {
  public:
    PlayerCom(boost::asio::ip::tcp::socket sock, Queue &queue);

  private:
    void receivePackets();

  private:
    boost::asio::ip::tcp::socket m_sock;
    Queue* m_queue;
  };
}

#endif // BITS_SERVER_PLAYER_COM
