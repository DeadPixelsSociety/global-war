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
      regiments.push_back({ originRegiment->ownerID, 1, destination, originRegiment->division });

      return;
    }

    assert(originRegiment->ownerID == destinationRegiment->ownerID);
    --originRegiment->count;
    ++destinationRegiment->count;
  }

  void Data::killUnit(gf::Vector2i position) {
    Regiment* regiment = getRegiment(position);
    assert(regiment != nullptr);

    --regiment->count;
  }

  void Data::cleanUpRegiments() {
    // Clear all empty regiment
    regiments.erase(std::remove_if(regiments.begin(), regiments.end(), [this](const Regiment &regiment){
      return regiment.count <= 0;
    }), regiments.end());
  }

  bool Data::isValidMove(gf::Vector2i origin, gf::Vector2i destination) {
    if (origin == destination) { // TODO: verify destination next to origin
      return false;
    }

    // TODO: Not in sea

    Regiment* destinationRegiment = getRegiment(destination);
    Regiment* originRegiment = getRegiment(origin);
    assert(originRegiment != nullptr);

    if (destinationRegiment != nullptr && destinationRegiment->ownerID == originRegiment->ownerID && destinationRegiment->division != originRegiment->division) {
      return false;
    }

    return true;
  }

}
