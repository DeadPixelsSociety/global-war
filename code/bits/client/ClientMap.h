#ifndef GW_CLIENT_MAP_H
#define GW_CLIENT_MAP_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/Path.h>

#include "../common/MapModel.h"

namespace gw {

  class ClientMap : public gf::Entity, public MapModel {
  public:
    ClientMap(const gf::Path& path);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
  };

}

#endif // GW_CLIENT_MAP_H
