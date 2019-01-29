#ifndef BITS_COMMON_MAP_MODEL_H
#define BITS_COMMON_MAP_MODEL_H

#include <gf/Array2D.h>
#include <gf/Model.h>
#include <gf/Path.h>

namespace gw {
  class MapModel : public gf::Model {
  public:
    static constexpr int Width = 32;
    static constexpr int Height = 32;

    MapModel(const gf::Path& path);

    gf::Vector2i getPosition(gf::Vector2f coords) const;

  protected:
    enum class TileType: int {
      Sea = 0,
      Land = 1,
    };

  protected:
    gf::Array2D<int, int> m_map;

  };
}

#endif // BITS_COMMON_MAP_MODEL_H
