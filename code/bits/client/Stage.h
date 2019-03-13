#ifndef BITS_CLIENT_STAGE_H
#define BITS_CLIENT_STAGE_H

#include <gf/Action.h>
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Window.h>

#include "GameState.h"

namespace gw {
  class Stage {
  public:
    Stage(gf::Window &window, gf::RenderWindow &renderer, GameState &gameState);

    virtual void loop() = 0;
    virtual void processPackets() = 0;

  protected:
    gf::Window &m_window;
    gf::RenderWindow &m_renderer;
    GameState &m_gameState;

    gf::ActionContainer m_actions;
    gf::Action m_closeWindowAction;
    gf::Action m_fullscreenAction;

    gf::ViewContainer m_views;
  };
}

#endif // BITS_CLIENT_STAGE_H
