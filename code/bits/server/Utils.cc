#include "Utils.h"

#include <limits>

#include "Singletons.h"

namespace gw {
  gf::Id generateId() {
    uint64_t min = std::numeric_limits<uint64_t>::min();
    uint64_t max = std::numeric_limits<uint64_t>::max();
    uint64_t number = gRandom().computeUniformInteger(min, max);

    return number;
  }
}
