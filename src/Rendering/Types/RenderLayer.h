#pragma once

#include <list>
#include "Presto/Objects/Entity.h"

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
