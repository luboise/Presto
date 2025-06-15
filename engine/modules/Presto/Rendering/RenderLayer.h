#pragma once

#include <list>

namespace Presto {
class Renderable;
using LayerList = std::list<Renderable*>;

class PRESTO_API RenderLayer {
   public:
    RenderLayer();
    virtual ~RenderLayer();

    void addRenderable(Renderable* renderable);
    LayerList _renderables;
};
}  // namespace Presto
