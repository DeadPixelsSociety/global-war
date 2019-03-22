#include "Data.h"

#include <gf/VectorOps.h>

namespace gw {

  Data::Data(const gf::Path& path)
  : map(path) {

  }

  Regiment* Data::getRegiment(gf::Vector2i position) {
    for (auto &regiment: regiments) {
      if (regiment.position == position) {
        return &regiment;
      }
    }

    return nullptr;
  }

  void Data::moveUnit(gf::Vector2i origin, gf::Vector2i destination) {
    Regiment* destinationRegiment = getRegiment(destination);
    Regiment* originRegiment = getRegiment(origin);
    assert(originRegiment != nullptr);

    if (destinationRegiment == nullptr) {
      --originRegiment->count;
      regiments.push_back({ originRegiment->ownerID, 1, destination });

      return;
    }

    assert(originRegiment->ownerID == destinationRegiment->ownerID);
    --originRegiment->count;
    ++destinationRegiment->count;
  }

  void Data::cleanUpRegiments() {
    // Clear all empty regiment
    regiments.erase(std::remove_if(regiments.begin(), regiments.end(), [this](const Regiment &regiment){
      return regiment.count <= 0;
    }), regiments.end());
  }

}
