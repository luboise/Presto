#pragma once

#include "Entity.h"
#include "Presto/Modules/Module.h"

namespace Presto {
    class PRESTO_API EntityManager : public Module<EntityManager> {
       public:
        static entity_t newEntity();
        static entity_t getEntityByID(id_t id);

        EntityManager(const EntityManager &) = delete;
        EntityManager(EntityManager &&) = delete;
        EntityManager &operator=(const EntityManager &) = delete;
        EntityManager &operator=(EntityManager &&) = delete;

        virtual ~EntityManager() = delete;

       private:
        static void destroyEntity(entity_t entity);
        static entity_id_t reserveId();

        static std::vector<entity_t> _entities;
        static std::map<entity_id_t, entity_t> _entityMap;
        static entity_id_t _currentId;
    };
}  // namespace Presto
