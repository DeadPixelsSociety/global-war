#ifndef GW_PACKET_H
#define GW_PACKET_H

#include <cstdint>

namespace gw {

  enum class PacketType : uint16_t {
    Ping,
    QuickMatch,
  };

  struct PacketPing {
    uint16_t sequence;
  };

  static constexpr int64_t InvalidPlayerID = -1;
  struct QuickMatch {
    int64_t playerID;
  };


  struct Packet {
    PacketType type;

    union {
      PacketPing ping;
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

      case PacketType::QuickMatch:
        ar | packet.quickMatch.playerID;
        break;
    }

    return ar;
  }
}

#endif // GW_PACKET_H
