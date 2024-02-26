#pragma once

#include <list>
#include "Presto/Modules/ObjectsModule/_EntityHeader.h"

namespace Presto {
    typedef std::list<Entity*> LayerList;

    class PRESTO_API RenderLayer {
       public:
        RenderLayer();
        virtual ~RenderLayer();

        void addEntity(Entity* entity);
        LayerList _entities;

        //    private:
    };
}  // namespace Presto