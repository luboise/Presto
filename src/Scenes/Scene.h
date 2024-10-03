#include "Presto/Objects/Entity.h"

namespace Presto {
    class Scene {
       public:
        inline void addEntity(entity_t entity) { _entities.push_back(entity); };

       private:
        std::vector<entity_t> _entities;
    };
}  // namespace Presto
