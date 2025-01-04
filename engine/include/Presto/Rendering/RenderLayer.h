#pragma once

#include <list>
#include "Presto/Components/Renderable.h"

namespace Presto {
    using LayerList = std::list<Renderable*>;

    class PRESTO_API RenderLayer {
       public:
        RenderLayer();
        virtual ~RenderLayer();

        void addRenderable(Renderable* renderable);
        LayerList _renderables;
    };
}  // namespace Presto
