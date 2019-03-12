#include "RegimentContainer.h"

namespace gw {

  const Regiment* RegimentContainer::operator[](gf::Vector2i position) const {
    auto it = army.find(position);

    if (it == army.end()) {
      return nullptr;
    }

    return std::addressof(*it);
  }

}
