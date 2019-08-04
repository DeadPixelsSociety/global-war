#ifndef _BITS_SERVER_MESSAGES_H
#define _BITS_SERVER_MESSAGES_H

#include <gf/Message.h>

#include "../common/Packets.h"

using namespace gf::literals;

namespace gw {

  struct PlayerInLobby: public gf::Message {
    static const gf::Id type = "PlayerInLobby"_id;
    gf::Id playerID;
  };

}

#endif // _BITS_SERVER_MESSAGES_H
