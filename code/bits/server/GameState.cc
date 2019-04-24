#include "GameState.h"

#include <iostream>

#include <gf/Log.h>

#include "Singletons.h"

namespace gw {

  namespace {
    constexpr gf::Time MoveDelay = gf::seconds(0.25f);
    constexpr float BonusAttack = 0.65f;
    constexpr float EqualAttack = 0.50f;
    constexpr float MalusAttack = 0.35f;
  }

  GameState::GameState()
  : data(gAssetManager().getAbsolutePath("map.txt")){

  }

  void GameState::update(gf::Time time) {
    // // DEBUG: Just print the map with regiments
    // for (int row = 0; row < MapData::Height; ++row) {
    //   for (int col = 0; col < MapData::Width; ++col) {
    //     if (data.getRegiment({ col, row }) != nullptr) {
    //       std::cout << 'X';
    //       continue;
    //     }
    //
    //     MapData::TileType type = static_cast<MapData::TileType>(data.map.getTile({ col, row }));
    //     switch (type) {
    //       case MapData::TileType::Sea:
    //         std::cout << '#';
    //         break;
    //       case MapData::TileType::Land:
    //         std::cout << ' ';
    //         break;
    //     }
    //   }
    //
    //   std::cout << '\n';
    // }

    for (auto &moveOrder: moveOrders) {
      moveOrder.countdown -= time;

      if (moveOrder.countdown <= gf::Time::zero()) {
        moveOrder.countdown += MoveDelay;

        Regiment* originRegiment = data.getRegiment(moveOrder.origin);
        assert(originRegiment != nullptr);
        Regiment* destinationRegiment = data.getRegiment(moveOrder.destination);
        assert(originRegiment != destinationRegiment);

        if (destinationRegiment != nullptr && originRegiment->ownerID != destinationRegiment->ownerID) {
          gf::Log::info("Attack regiment form {%d,%d} to {%d,%d}\n",
            moveOrder.origin.x, moveOrder.origin.y,
            moveOrder.destination.x, moveOrder.destination.y);

          // Handle attack
          Packet packet;
          packet.type = PacketType::KillUnit;

          // Compute the bonus
          // Horseman > Swordsman
          // Swordsman > Lancer
          // Lancer > Horseman
          float modifier = EqualAttack;
          switch (originRegiment->division) {
            case Division::Horseman:
              switch (destinationRegiment->division) {
                case Division::Swordsman:
                  modifier = BonusAttack;
                  break;
                case Division::Lancer:
                  modifier = MalusAttack;
                  break;
                default:
                  break;
              }
              break;

            case Division::Swordsman:
              switch (destinationRegiment->division) {
                case Division::Lancer:
                  modifier = BonusAttack;
                  break;
                case Division::Horseman:
                  modifier = MalusAttack;
                  break;
                default:
                  break;
              }
              break;

            case Division::Lancer:
              switch (destinationRegiment->division) {
                case Division::Horseman:
                  modifier = BonusAttack;
                  break;
                case Division::Swordsman:
                  modifier = MalusAttack;
                  break;
                default:
                  break;
              }
              break;
          }

          gf::Log::debug("Modifier attack: %f\n", modifier);

          if (gRandom().computeBernoulli(modifier)) {
            --destinationRegiment->count;
            packet.killUnit.position = destinationRegiment->position;
          } else {
            --originRegiment->count;
            packet.killUnit.position = originRegiment->position;
          }

          pendingPackets.push_back(packet);

          continue;
        }

        gf::Log::info("Move unit form {%d,%d} to {%d,%d}\n",
          moveOrder.origin.x, moveOrder.origin.y,
          moveOrder.destination.x, moveOrder.destination.y);
        data.moveUnit(moveOrder.origin, moveOrder.destination);

        // Create packet
        Packet packet;
        packet.type = PacketType::MoveUnit;
        packet.moveUnit.origin = moveOrder.origin;
        packet.moveUnit.destination = moveOrder.destination;

        pendingPackets.push_back(packet);
      }
    }

    // Clear all empty move order
    moveOrders.erase(std::remove_if(moveOrders.begin(), moveOrders.end(), [this](const MoveOrder &moveOrder){
      Regiment* regiment = data.getRegiment(moveOrder.origin);
      assert(regiment != nullptr);

      return regiment->count <= 0;
    }), moveOrders.end());

    // Clear all empty regiment
    data.cleanUpRegiments();
    // I'am not sure how develop the code
  }
}
