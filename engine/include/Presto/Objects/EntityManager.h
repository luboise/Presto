#pragma once

#include "Entity.h"
#include "Presto/Module.h"
#include "Presto/Objects/Component.h"

namespace Presto {
    class PRESTO_API EntityManager : public Module<EntityManager> {
        using EntityMap = std::map<entity_id_t, entity_ptr>;

       public:
        static entity_ptr newEntity();
        static entity_ptr getEntityByID(entity_id_t id);

        EntityMap &findAll() { return entityMap_; };

        std::vector<entity_ptr> findWhere(auto filter) {
            return entityMap_ | std::views::values | std::views::filter(filter);
        }

        std::vector<component_ptr> findComponentsWhere(auto filter) {
            return components_ | std::views::filter(filter);
        }

        void Update() override;

        EntityManager(const EntityManager &) = delete;
        EntityManager(EntityManager &&) = delete;
        EntityManager &operator=(const EntityManager &) = delete;
        EntityManager &operator=(EntityManager &&) = delete;

        virtual ~EntityManager() = delete;

       private:
        static void destroyEntity(entity_ptr entity);
        static entity_id_t reserveId();

        static std::vector<entity_ptr> entities_;
        static std::vector<component_ptr> components_;
        static EntityMap entityMap_;
        static entity_id_t _currentId;
    };
}  // namespace Presto
