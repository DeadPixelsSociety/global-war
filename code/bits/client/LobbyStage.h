#ifndef BITS_CLIENT_LOBBY_STAGE_H
#define BITS_CLIENT_LOBBY_STAGE_H

#include <gf/EntityContainer.h>
#include <gf/ResourceManager.h>
#include <gf/Views.h>

#include "Stage.h"
#include "WaitScreen.h"

namespace gw {
  class LobbyStage : public Stage {
  public:
    LobbyStage(gf::Window &window, gf::RenderWindow &renderer, ClientModel &clientModel, gf::ResourceManager &resources);

    void loop() override;
    void processPackets() override;

  private:
    gf::ScreenView m_hudView;

    gf::EntityContainer m_entities;
    WaitScreen m_waitScreen;

    bool m_gameFound;
  };
}

#endif // BITS_CLIENT_LOBBY_STAGE_H
