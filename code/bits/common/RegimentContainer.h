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

  inline bool operator<(const Regiment& lhs, const Regiment& rhs) {
    return std::tie(lhs.position.x, lhs.position.y) < std::tie(rhs.position.x, rhs.position.y);
  }

  inline bool operator<(gf::Vector2i lhs, const Regiment& rhs) {
    return std::tie(lhs.x, lhs.y) < std::tie(rhs.position.x, rhs.position.y);
  }

  inline bool operator<(const Regiment& lhs, gf::Vector2i rhs) {
    return std::tie(lhs.position.x, lhs.position.y) < std::tie(rhs.x, rhs.y);
  }

  class RegimentContainer {
  public:
    const Regiment* operator[](gf::Vector2i position) const;

    std::set<Regiment, std::less<>>& operator()();
    const std::set<Regiment, std::less<>>& operator()() const;

  private:
    std::set<Regiment, std::less<>> m_army;
  };


}

#endif // BITS_COMMON_REGIMENT_CONTAINER_H
