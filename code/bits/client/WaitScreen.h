#ifndef BITS_CLIENT_WAIT_SCREEN_H
#define BITS_CLIENT_WAIT_SCREEN_H

#include <gf/Entity.h>
#include <gf/Font.h>

namespace gw {
  class WaitScreen: public gf::Entity {
  public:
    WaitScreen(gf::Font &font);

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font& m_font;
    gf::Time m_elapsedTime;
  };
}

#endif // BITS_CLIENT_WAIT_SCREEN_H
