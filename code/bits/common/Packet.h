#ifndef GW_PACKET_H
#define GW_PACKET_H

#include <array>
#include <cstdint>

#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

#include "Regiment.h"

namespace gw {

  static constexpr int MaxPlayers = 8;

  enum class PacketType : uint16_t {
    NewPlayer,
    QuickMatch,
    JoinGame,
    AckJoinGame,            // No struct data, just acknowledge
    CreateRegiment,
    MoveRegiment,
    MoveUnit,
    KillUnit,
    InitializePlayer,
    WinGame,
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
    Division division;
  };

  struct MoveRegiment {
    gf::Id playerID;
    gf::Vector<int32_t, 2> regimentOrigin;
    gf::Vector<int32_t, 2> regimentDestination;
  };

  struct MoveUnit {
    gf::Vector<int32_t, 2> origin;
    gf::Vector<int32_t, 2> destination;
  };

  struct KillUnit {
    gf::Vector<int32_t, 2> position;
  };

  struct InitializePlayer {
    gf::Id playerID;
    gf::Vector<int32_t, 2> position;
  };

  struct WinGame {
    gf::Id winner;
  };

  struct Packet {
    PacketType type;

    union {
      NewPlayer newPlayer;
      QuickMatch quickMatch;
      JoinGame joinGame;
      CreateRegiment createRegiment;
      MoveRegiment moveRegiment;
      MoveUnit moveUnit;
      KillUnit killUnit;
      InitializePlayer initializePlayer;
      WinGame winGame;
    };
  };

  template<class Archive>
  Archive& operator|(Archive& ar, Packet& packet) {
    ar | packet.type;

    switch (packet.type) {
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

      case PacketType::AckJoinGame:
        break;

      case PacketType::CreateRegiment:
        ar | packet.createRegiment.count;
        ar | packet.createRegiment.position;
        ar | packet.createRegiment.ownerID;
        ar | packet.createRegiment.division;
        break;

      case PacketType::MoveRegiment:
        ar | packet.moveRegiment.playerID;
        ar | packet.moveRegiment.regimentOrigin;
        ar | packet.moveRegiment.regimentDestination;
        break;

      case PacketType::MoveUnit:
        ar | packet.moveUnit.origin;
        ar | packet.moveUnit.destination;
        break;

      case PacketType::KillUnit:
        ar | packet.killUnit.position;
        break;

      case PacketType::InitializePlayer:
        ar | packet.initializePlayer.playerID;
        ar | packet.initializePlayer.position;
        break;

      case PacketType::WinGame:
        ar | packet.winGame.winner;
        break;
    }

    return ar;
  }
}

#endif // GW_PACKET_H
