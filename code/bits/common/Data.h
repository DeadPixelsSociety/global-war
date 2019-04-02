#ifndef BITS_COMMON_DATA_H
#define BITS_COMMON_DATA_H

#include <vector>

#include <gf/Vector.h>

#include "Regiment.h"
#include "MapData.h"

namespace gw {

  struct Data {
    Data(const gf::Path& path);

    MapData map;

    std::vector<Regiment> regiments;

    Regiment* getRegiment(gf::Vector2i position);
    void moveUnit(gf::Vector2i origin, gf::Vector2i destination);
    void killUnit(gf::Vector2i position);
    void cleanUpRegiments();
  };

}

#endif // BITS_COMMON_DATA_H
