#ifndef GW_PACKET_H
#define GW_PACKET_H

#include <cstdint>

#include <gf/Id.h>

namespace gw {

  enum class PacketType : uint16_t {
    Ping,
    NewPlayer,
    QuickMatch,
  };

  struct PacketPing {
    uint16_t sequence;
  };

  static constexpr gf::Id InvalidPlayerID = gf::InvalidId;
  struct NewPlayer {
    gf::Id playerID;
  };

  struct QuickMatch {
    gf::Id playerID;
  };


  struct Packet {
    PacketType type;

    union {
      PacketPing ping;
      NewPlayer newPlayer;
      QuickMatch quickMatch;
    };
  };


  template<class Archive>
  Archive& operator|(Archive& ar, Packet& packet) {
    ar | packet.type;

    switch (packet.type) {
      case PacketType::Ping:
        ar | packet.ping.sequence;
        break;

      case PacketType::NewPlayer:
        ar | packet.newPlayer.playerID;
        break;

      case PacketType::QuickMatch:
        ar | packet.quickMatch.playerID;
        break;
    }

    return ar;
  }
}

#endif // GW_PACKET_H
