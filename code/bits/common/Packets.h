#ifndef GW_PACKETS_H
#define GW_PACKETS_H

#include <array>
#include <cstdint>

#include <gf/Id.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

#include "Regiment.h"

namespace gw {

  static constexpr int MaxPlayers = 8;
  static constexpr gf::Id InvalidPlayerID = gf::InvalidId;

  // Packet for the server
  enum class PacketLobbyServerType {
    RequestMatch, // Ask to join a game
    ConfirmJoinGame, // Acknowledge join new game session
  };

  enum class PacketGameServerType {
    MoveRegiment, // Ask for regiment move
  };

  // Packet for the client
  enum class PacketLobbyClientType {
    CreatePlayer, // Send the player ID
    JoinGame, // Join a new session game
  };

  enum class PacketGameClientType {
    InitializePlayer, // Set all initial information for the player
    CreateRegiment, // Create a new regiment
    MoveUnit, // Move a unit
    KillUnit, // Kill a unit
    DeclareWinner, // Inform the players of winner
  };

  // Lobby Server
  struct RequestMatch {
    gf::Id playerID;
  };

  /*struct ConfirmJoinGame {
    // No data needed
  };*/

  // Game Server
  struct MoveRegiment {
    gf::Id playerID;
    gf::Vector<int32_t, 2> regimentOrigin;
    gf::Vector<int32_t, 2> regimentDestination;
  };

  // Client Lobby
  struct CreatePlayer {
    gf::Id playerID;
  };

  struct JoinGame {
    gf::Id gameID;
    int32_t nbPlayers;
    std::array<gf::Id, MaxPlayers> playersID;
  };

  // Client game
  struct InitializePlayer {
    gf::Id playerID;
    gf::Vector<int32_t, 2> position;
  };

  struct CreateRegiment {
    int32_t count;
    gf::Vector<int32_t, 2> position;
    gf::Id ownerID;
    Division division;
  };

  struct MoveUnit {
    gf::Vector<int32_t, 2> origin;
    gf::Vector<int32_t, 2> destination;
  };

  struct KillUnit {
    gf::Vector<int32_t, 2> position;
  };

  struct DeclareWinner {
    gf::Id winner;
  };

  struct PacketLobbyServer {
    PacketLobbyServerType type;

    union {
      RequestMatch requestMatch;
      // ConfirmJoinGame confirmJoinGame;
    };
  };

  struct PacketGameServer {
    PacketGameServerType type;

    union {
      MoveRegiment moveRegiment;
    };
  };

  struct PacketLobbyClient {
    PacketLobbyClientType type;

    union {
      CreatePlayer createPlayer;
      JoinGame joinGame;
    };
  };

  struct PacketGameClient {
    PacketGameClientType type;

    union {
      InitializePlayer initializePlayer;
      CreateRegiment createRegiment;
      MoveUnit moveUnit;
      KillUnit killUnit;
      DeclareWinner declareWinner;
    };
  };

  template<class Archive>
  Archive& operator|(Archive& ar, PacketLobbyServer& packet) {
    ar | packet.type;

    switch (packet.type) {
      case PacketLobbyServerType::RequestMatch:
        ar | packet.requestMatch.playerID;
        break;

      case PacketLobbyServerType::ConfirmJoinGame:
        // No data structure
        break;
    }

    return ar;
  }

  template<class Archive>
  Archive& operator|(Archive& ar, PacketGameServer& packet) {
    ar | packet.type;

    switch (packet.type) {
      case PacketGameServerType::MoveRegiment:
        ar | packet.moveRegiment.playerID;
        ar | packet.moveRegiment.regimentOrigin;
        ar | packet.moveRegiment.regimentDestination;
        break;
    }

    return ar;
  }

  template<class Archive>
  Archive& operator|(Archive& ar, PacketLobbyClient& packet) {
    ar | packet.type;

    switch (packet.type) {
      case PacketLobbyClientType::CreatePlayer:
        ar | packet.createPlayer.playerID;
        break;

      case PacketLobbyClientType::JoinGame:
        ar | packet.joinGame.gameID;
        ar | packet.joinGame.nbPlayers;
        ar | packet.joinGame.playersID;
        break;
    }

    return ar;
  }

  template<class Archive>
  Archive& operator|(Archive& ar, PacketGameClient& packet) {
    ar | packet.type;

    switch (packet.type) {
      case PacketGameClientType::InitializePlayer:
      ar | packet.initializePlayer.playerID;
      ar | packet.initializePlayer.position;
      break;

      case PacketGameClientType::CreateRegiment:
        ar | packet.createRegiment.count;
        ar | packet.createRegiment.position;
        ar | packet.createRegiment.ownerID;
        ar | packet.createRegiment.division;
        break;

      case PacketGameClientType::MoveUnit:
        ar | packet.moveUnit.origin;
        ar | packet.moveUnit.destination;
        break;

      case PacketGameClientType::KillUnit:
        ar | packet.killUnit.position;
        break;

      case PacketGameClientType::DeclareWinner:
        ar | packet.declareWinner.winner;
        break;
    }

    return ar;
  }
}

#endif // GW_PACKETS_H
