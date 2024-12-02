#pragma once

#include "Presto/Module.h"
#include "Presto/Objects/Entity.h"

namespace Presto {
    using entity_t = Entity *;

    class PRESTO_API EntityManager : public Module<EntityManager> {
       public:
        // Intellisense for constructor call
        template <typename T, typename... ArgT>
        static std::enable_if_t<std::is_constructible_v<T, ArgT...>, entity_t>
        newEntity(ArgT... args);

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
