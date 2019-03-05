#ifndef BITS_CLIENT_MODEL_H
#define BITS_CLIENT_MODEL_H

#include <vector>

#include <gf/Color.h>
#include <gf/Id.h>
#include <gf/Queue.h>

#include "../common/Packet.h"
#include "../common/ThreadCom.h"

namespace gw {
  static constexpr gf::Vector2u InitialScreenSize(1024, 576);

  struct ClientModel {
    ClientModel(char *hostname, char* port);

    // Network
    gf::Queue<Packet> comQueue;
    ThreadCom threadCom;
    void quickMacth();

    // Players management
    gf::Id currentPlayerID;
    std::vector<gf::Id> allPlayerID;

    gf::Color4f getPlayerColor();
    gf::Color4f getPlayerColor(gf::Id playerID);

  };
}

#endif // BITS_CLIENT_MODEL_H
