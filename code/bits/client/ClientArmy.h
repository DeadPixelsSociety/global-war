#ifndef GW_CLIENT_ARMY_H
#define GW_CLIENT_ARMY_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Vector.h>

namespace gw {

  struct Regiment {
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

  class ClientArmy : public gf::Entity {
  public:
    ClientArmy(gf::ResourceManager& resources);

    const Regiment* getRegiment(gf::Vector2i position) const;

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;


  private:
    gf::Font& m_font;

    std::set<Regiment, std::less<>> m_army;
  };

}

#endif // GW_CLIENT_ARMY_H
