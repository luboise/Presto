#pragma once

#include "Presto/Module.h"
#include "Presto/Objects/Entity.h"

namespace Presto {
    typedef Entity* entity_t;

    class PRESTO_API EntityManager : public Module {
       public:
        static entity_t newEntity();
        static entity_t getEntityByID(id_t id);

       private:
        static void destroyEntity(entity_t entity);
        static entity_id_t reserveId();

        static std::vector<entity_t> _entities;
        static std::map<entity_id_t, entity_t> _entityMap;
        static entity_id_t _currentId;
    };
}  // namespace Presto
