#ifndef GW_CLIENT_ARMY_H
#define GW_CLIENT_ARMY_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Vector.h>

namespace gw {

  class ClientArmy : public gf::Entity {
  public:
    ClientArmy(gf::ResourceManager& resources);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font& m_font;

    struct Regiment {
      int count;
      gf::Vector2i position;
    };

    std::vector<Regiment> m_army;
  };

}

#endif // GW_CLIENT_ARMY_H
