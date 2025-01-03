#pragma once

#include <list>
#include "PrestoCore/Components/Renderable.h"

namespace Presto {
    typedef std::list<Renderable*> LayerList;

    class PRESTO_API RenderLayer {
       public:
        RenderLayer();
        virtual ~RenderLayer();

        void addRenderable(Renderable* renderable);
        LayerList _renderables;
    };
}  // namespace Presto
