#ifndef BITS_COMMON_MAP_DATA_H
#define BITS_COMMON_MAP_DATA_H

#include <gf/Array2D.h>
#include <gf/Path.h>

namespace gw {
  class MapData {
  public:
    static constexpr int Width = 32;
    static constexpr int Height = 32;

    MapData(const gf::Path& path);

    gf::Vector2i getTileCoordinate(gf::Vector2f screenCoordinate) const;
    gf::PositionRange<int> getPositionRange() const;
    int getTile(gf::Vector2i position) const;

  public:
    enum class TileType: int {
      Sea = 0,
      Land = 1,
    };

  private:
    gf::Array2D<int, int> m_map;

  };
}

#endif // BITS_COMMON_MAP_DATA_H
