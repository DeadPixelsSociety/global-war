#ifndef BITS_CLIENT_REGIMENT_RENDER_H
#define BITS_CLIENT_REGIMENT_RENDER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Vector.h>

#include "../common/Data.h"

#include "GameState.h"

namespace gw {

  class RegimentRender: public gf::Entity {
  public:
    RegimentRender(GameState &gameState);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font& m_font;
    GameState &m_gameState;
  };

}

#endif // BITS_CLIENT_REGIMENT_RENDER_H
