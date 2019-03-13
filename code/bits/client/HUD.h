#ifndef BITS_CLIENT_HUD_H
#define BITS_CLIENT_HUD_H

#include <gf/Entity.h>

#include "GameState.h"

namespace gw {
  class HUD: public gf::Entity {
  public:
    HUD(GameState &gameState);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    GameState& m_gameState;
  };
}

#endif
