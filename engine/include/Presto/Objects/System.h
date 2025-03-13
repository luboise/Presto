#pragma once

#include "Entity.h"

namespace Presto {
class PRESTO_API System {
   public:
    void Update() {
        for (Entity* entity : _entities) {
            UpdateEntity(*entity);
        }
    }
    virtual void UpdateEntity(Entity& e) = 0;

   private:
    std::vector<EntityPtr> _entities;
};
}  // namespace Presto
