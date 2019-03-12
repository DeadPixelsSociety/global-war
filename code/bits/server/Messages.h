#ifndef _BITS_SERVER_MESSAGES_H
#define _BITS_SERVER_MESSAGES_H

#include <gf/Message.h>

using namespace gf::literals;

namespace gw {

  struct PlayerDisconnected : public gf::Message {
    static const gf::Id type = "PlayerDisconnected"_id;
    gf::Id playerID; // ID of next player
  };

}

#endif // _BITS_SERVER_MESSAGES_H
