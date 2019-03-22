#ifndef BITS_COMMON_REGIMENT_CONTAINER_H
#define BITS_COMMON_REGIMENT_CONTAINER_H

#include <set>
#include <tuple>

#include <gf/Id.h>
#include <gf/Vector.h>

namespace gw {

  struct Regiment {
    gf::Id ownerID;
    int count;
    gf::Vector2i position;
  };

}

#endif // BITS_COMMON_REGIMENT_CONTAINER_H
