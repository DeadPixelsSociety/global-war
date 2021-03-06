#ifndef BITS_COMMON_REGIMENT_CONTAINER_H
#define BITS_COMMON_REGIMENT_CONTAINER_H

#include <set>
#include <tuple>

#include <gf/Id.h>
#include <gf/Vector.h>

namespace gw {
  enum class Division : uint8_t {
    Horseman,
    Swordsman,
    Lancer,
  };

  struct Regiment {
    gf::Id ownerID;
    int count;
    gf::Vector2i position;
    Division division;
  };

}

#endif // BITS_COMMON_REGIMENT_CONTAINER_H
