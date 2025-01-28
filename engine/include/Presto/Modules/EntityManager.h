#pragma once

#include <functional>
#include "Module.h"

#include "Presto/Objects/Component.h"
#include "Presto/Objects/Entity.h"

#include "Presto/Objects/Figure.h"

namespace Presto {
    class PRESTO_API EntityManager : public Module<EntityManager> {
        friend class Application;
        friend Figure::~Figure();

        using EntityMap = std::map<entity_id_t, entity_ptr>;

       public:
        [[nodiscard]] entity_ptr newEntity(
            const entity_name_t &name = "Entity");

        entity_ptr getEntityByID(entity_id_t id);

        std::vector<entity_ptr> findAll();

        std::vector<entity_ptr> findWhere(auto filter);

        std::vector<component_ptr> findComponentsWhere(auto filter);

        void addTagToEntity(Entity &entity, entity_tag_name_t tag);
        void update() override;

        entity_tag_id_t createTag(const entity_tag_name_t &tagName);
        [[nodiscard]] entity_tag_id_t getTagId(
            const entity_tag_name_t &tagName) const;

        [[nodiscard]] bool exists(entity_id_t id) const;

        EntityManager(const EntityManager &) = delete;
        EntityManager(EntityManager &&) = delete;
        EntityManager &operator=(const EntityManager &) = delete;
        EntityManager &operator=(EntityManager &&) = delete;

        // ~EntityManager() = default;

        std::vector<Entity *> newEntities(PR_SIZE count);

        /*
template <typename T = Entity, typename... Args>
T *newEntity(Args &&...args) {
    entity_id_t new_id{EntityManager::reserveId()};

    PR_CORE_ASSERT(exists(new_id),
                   "Attempted to create entity using existing id: {}",
                   new_id);

    auto new_entity{entity_unique_ptr(
        new T(new_id, args...),

        [this](Entity *entity) { this->destroyEntity(entity); })};

    auto *new_transform{newComponent<Transform>()};
    new_entity->setComponent(new_transform);

    entityMap_.emplace(new_id, std::move(new_entity));
    return entityMap_[new_id].get();
};
        */

        // TODO: Consider making this private. It's not a huge deal either way,
        // and people can just choose which one they use.
        template <typename T, typename... Args>
        T *newComponent(Args &&...args) {
            // std::unique_ptr<Component> new_component{new T};
            auto new_component{
                std::shared_ptr<Component>(new T(std::forward<Args>(args)...))};

            component_id_t new_id{reserveId()};
            new_component->id_ = new_id;

            components_.emplace(new_id, std::move(new_component));

            return static_cast<T *>(components_[new_id].get());
        };

       private:
        EntityManager() = default;

        static void init();
        static void shutdown();

        void collectGarbage();

        void destroyEntity(entity_ptr entity);
        entity_id_t reserveId();

        using entity_unique_ptr =
            std::unique_ptr<Entity, std::function<void(Entity *)>>;

        std::vector<entity_ptr> entities_;
        std::map<component_id_t, std::shared_ptr<Component>> components_;
        // static std::vector<component_ptr> components_;
        std::map<entity_id_t, entity_unique_ptr> entityMap_;

        std::vector<entity_tag_name_t> tagMap_;

        entity_id_t _currentId{1};
    };
}  // namespace Presto
