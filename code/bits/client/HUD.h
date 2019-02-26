#ifndef BITS_CLIENT_HUD_H
#define BITS_CLIENT_HUD_H

#include <gf/Entity.h>

#include "ClientModel.h"

namespace gw {
  class HUD: public gf::Entity {
  public:
    HUD(ClientModel &clientModel);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    ClientModel &m_clientModel;
  };
}

#endif
