#include "Stage.h"

namespace gw {
  Stage::Stage(gf::Window &window, gf::RenderWindow &renderer, GameState &gameState)
  : m_window(window)
  , m_renderer(renderer)
  , m_gameState(gameState)
  , m_closeWindowAction("Close window")
  , m_fullscreenAction("Fullscreen") {
      m_closeWindowAction.addCloseControl();
      m_closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
      m_actions.addAction(m_closeWindowAction);

      m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
      m_actions.addAction(m_fullscreenAction);
  }
}
