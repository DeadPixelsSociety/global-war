#ifndef BITS_SERVER_GAME_STATE_H
#define BITS_SERVER_GAME_STATE_H

#include <deque>

#include <gf/Model.h>

#include "../common/Data.h"
#include "../common/Packets.h"

namespace gw {
  struct MoveOrder {
    gf::Vector2i origin;
    gf::Vector2i destination;
    gf::Time countdown;
  };

  struct GameState: public gf::Model {
    GameState();

    void update(gf::Time time) override;
    void checkEndCondition(const std::vector<gf::Id> &players);

    Data data;
    std::vector<MoveOrder> moveOrders;

    std::deque<PacketGameClient> pendingPackets;
  };

}

#endif // BITS_SERVER_GAME_STATE_H
