#ifndef GW_PACKET_H
#define GW_PACKET_H

#include <array>
#include <cstdint>

#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

namespace gw {

  static constexpr int MaxPlayers = 8;

  enum class PacketType : uint16_t {
    Ping,
    NewPlayer,
    QuickMatch,
    JoinGame,
    CreateRegiment,
    MoveRegiment,
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

  struct JoinGame {
    gf::Id gameID;
    int32_t nbPlayers;
    std::array<gf::Id, MaxPlayers> playersID;
  };

  struct CreateRegiment {
    int32_t count;
    gf::Vector<int32_t, 2> position;
    gf::Id ownerID;
  };

  struct MoveRegiment {
    gf::Id playerID;
    gf::Vector<int32_t, 2> regimentOrigin;
    gf::Vector<int32_t, 2> regimentDestination;
  };


  struct Packet {
    PacketType type;

    union {
      PacketPing ping;
      NewPlayer newPlayer;
      QuickMatch quickMatch;
      JoinGame joinGame;
      CreateRegiment createRegiment;
      MoveRegiment moveRegiment;
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

      case PacketType::JoinGame:
        ar | packet.joinGame.gameID;
        ar | packet.joinGame.nbPlayers;
        ar | packet.joinGame.playersID;
        break;

      case PacketType::CreateRegiment:
        ar | packet.createRegiment.count;
        ar | packet.createRegiment.position;
        ar | packet.createRegiment.ownerID;
        break;

      case PacketType::MoveRegiment:
        ar | packet.moveRegiment.playerID;
        ar | packet.moveRegiment.regimentOrigin;
        ar | packet.moveRegiment.regimentDestination;
        break;
    }

    return ar;
  }
}

#endif // GW_PACKET_H
