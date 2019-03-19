#include "GameState.h"

#include <iostream>

#include <gf/Log.h>

#include "Singletons.h"

namespace gw {

  GameState::GameState()
  : data(gAssetManager().getAbsolutePath("map.txt")){

  }

  void GameState::update(gf::Time time) {
    // DEBUG: Just print the map with regiments
    for (int row = 0; row < MapData::Height; ++row) {
      for (int col = 0; col < MapData::Width; ++col) {
        if (data.getRegiment({ col, row }) != nullptr) {
          std::cout << 'X';
          continue;
        }

        MapData::TileType type = static_cast<MapData::TileType>(data.map.getTile({ col, row }));
        switch (type) {
          case MapData::TileType::Sea:
            std::cout << '#';
            break;
          case MapData::TileType::Land:
            std::cout << ' ';
            break;
        }
      }

      std::cout << '\n';
    }
  }
}
