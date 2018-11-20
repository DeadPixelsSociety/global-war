#ifndef GW_PACKET_H
#define GW_PACKET_H

#include <cstdint>

namespace gw {

  enum class PacketType : uint16_t {
    Ping,
  };

  struct PacketPing {
    uint16_t sequence;
  };


  struct Packet {
    PacketType type;

    union {
      PacketPing ping;
    };
  };


  template<class Archive>
  Archive& operator|(Archive& ar, Packet& packet) {
    ar | packet.type;

    switch (packet.type) {
      case PacketType::Ping:
        ar | packet.ping.sequence;
        break;
    }

    return ar;
  }
}

#endif // GW_PACKET_H
