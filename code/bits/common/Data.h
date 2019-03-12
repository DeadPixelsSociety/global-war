#ifndef BITS_COMMON_DATA_H
#define BITS_COMMON_DATA_H

#include <set>

#include <gf/Vector.h>

#include "Regiment.h"

namespace gw {

  struct Data {
    std::set<Regiment, std::less<>> regiments;

    const Regiment* getRegiment(gf::Vector2i position) const;
  };

}

#endif // BITS_COMMON_DATA_H
