#ifndef BITS_COMMON_MAP_DATA_H
#define BITS_COMMON_MAP_DATA_H

#include <gf/Array2D.h>
#include <gf/Path.h>
#include <gf/Random.h>

namespace gw {
  class MapData {
  public:
    enum class TileType: int {
      Sea = 0,
      Land = 1,
    };

  public:
    static constexpr int Width = 32;
    static constexpr int Height = 32;

    MapData(const gf::Path& path);

    gf::Vector2i getTileCoordinate(gf::Vector2f screenCoordinate) const;
    gf::PositionRange<int> getPositionRange() const;
    TileType getTile(gf::Vector2i position) const;
    std::vector<gf::Vector2i> generateInitialPositions(gf::Random &random, size_t nbPlayers) const;
    std::vector<gf::Vector2i> getEmptyNeighborPositions(gf::Vector2i position) const;

  private:
    gf::Array2D<TileType, int> m_map;

  };
}

#endif // BITS_COMMON_MAP_DATA_H
