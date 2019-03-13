#ifndef BITS_CLIENT_MAP_RENDER_H
#define BITS_CLIENT_MAP_RENDER_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/Path.h>

#include "GameState.h"

namespace gw {

  class MapRender: public gf::Entity {
  public:
    MapRender(GameState &gameState);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    GameState &m_gameState;
  };

}

#endif // BITS_CLIENT_MAP_RENDER_H
