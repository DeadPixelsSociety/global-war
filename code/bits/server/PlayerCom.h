#ifndef BITS_SERVER_PLAYER_COM
#define BITS_SERVER_PLAYER_COM

#include <boost/asio.hpp>

namespace gw {
  class PlayerCom {
  public:
    PlayerCom(boost::asio::ip::tcp::socket sock);

  private:
    void receivePackets();

  private:
    boost::asio::ip::tcp::socket m_sock;
  };
}

#endif // BITS_SERVER_PLAYER_COM
