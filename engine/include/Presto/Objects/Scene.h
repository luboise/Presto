#pragma once

#include "Presto/Objects/Entity.h"

namespace Presto {
    using scene_id_t = PR_STRING_ID;

    class Scene {
       public:
        static constexpr Scene* INVALID = nullptr;

        inline void addEntity(entity_ptr entity) {
            _entities.push_back(entity);
        };

       private:
        scene_id_t _name;
        std::vector<entity_ptr> _entities;
    };
}  // namespace Presto
