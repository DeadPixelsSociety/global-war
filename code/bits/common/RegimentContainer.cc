#include "RegimentContainer.h"

namespace gw {

  const Regiment* RegimentContainer::operator[](gf::Vector2i position) const {
    auto it = m_army.find(position);

    if (it == m_army.end()) {
      return nullptr;
    }

    return std::addressof(*it);
  }

  std::set<Regiment, std::less<>>& RegimentContainer::operator()() {
    return m_army;
  }

  const std::set<Regiment, std::less<>>& RegimentContainer::operator()() const {
    return m_army;
  }

}
