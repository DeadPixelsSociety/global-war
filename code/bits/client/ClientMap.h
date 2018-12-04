#ifndef GW_CLIENT_MAP_H
#define GW_CLIENT_MAP_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/Path.h>

namespace gw {

  class ClientMap : public gf::Entity {
  public:
    static constexpr int Width = 32;
    static constexpr int Height = 32;

    ClientMap(const gf::Path& path);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Array2D<int, int> m_map;
  };

}

#endif // GW_CLIENT_MAP_H
