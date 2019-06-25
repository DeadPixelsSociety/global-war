#ifndef _BITS_CLIENT_MESSAGES_H
#define _BITS_CLIENT_MESSAGES_H

#include <gf/Message.h>

using namespace gf::literals;

namespace gw {

  struct GameStart: public gf::Message {
    static const gf::Id type = "GameStart"_id;
  };

}

#endif // _BITS_SERVER_MESSAGES_H
