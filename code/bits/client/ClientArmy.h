#ifndef GW_CLIENT_ARMY_H
#define GW_CLIENT_ARMY_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Vector.h>

#include "../common/Packet.h"
#include "../common/RegimentContainer.h"

#include "ClientModel.h"

namespace gw {

  class ClientArmy : public gf::Entity {
  public:
    ClientArmy(gf::ResourceManager& resources, ClientModel &clientModel);

    [[deprecated("Move the regiment container to ClientModel")]]
    const Regiment* getRegiment(gf::Vector2i position) const;
    void createRegiment(int count, gf::Vector2i position, gf::Id ownerID);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font& m_font;
    ClientModel &m_clientModel;

    RegimentContainer m_regiments;
  };

}

#endif // GW_CLIENT_ARMY_H
