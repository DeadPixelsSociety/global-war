#include "Data.h"

namespace gw {

  Data::Data(const gf::Path& path)
  : map(path) {

  }

  const Regiment* Data::getRegiment(gf::Vector2i position) const {
    auto it = regiments.find(position);

    if (it == regiments.end()) {
      return nullptr;
    }

    return std::addressof(*it);
  }

}
