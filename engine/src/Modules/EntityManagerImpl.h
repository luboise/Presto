#pragma once

#include "Module.h"

#include "Presto/Core/ViewHandling.h"
#include "Presto/Objects/Component.h"
#include "Presto/Objects/Entity.h"

#include "Presto/Objects/EntityManager.h"
#include "Presto/Objects/Figure.h"

namespace Presto {
class PRESTO_API EntityManagerImpl final : public Module<EntityManagerImpl>,
                                           public EntityManager {
    MODULE_FUNCTIONS(EntityManagerImpl);

    friend class Application;
    friend class Module;

    friend void Entity::destroy();

    // friend class Module<EntityManagerImpl>;

    friend Figure::~Figure();

    using EntityMap = std::map<entity_id_t, EntityPtr>;

   public:
    using ComponentMap = std::map<component_id_t, GenericComponentPtr>;

    [[nodiscard]] EntityPtr newEntity(const entity_name_t& name = "Entity");
    std::vector<EntityPtr> newEntities(PR_SIZE count);

    EntityPtr getEntityByID(entity_id_t id);

    std::vector<EntityPtr> findAll();

    std::vector<EntityPtr> findWhere(auto filter);

    ComponentSearchResults findComponentsWhere(const ComponentFilter& =
                                                   [](auto&) { return true; });

    template <ComponentType T>
    std::vector<ComponentPtr<T>>& findComponentsByType() {
        return *getComponentList<T>();
    }

    template <ComponentType T>
    std::vector<T>* getComponents() {}

    void addTagToEntity(Entity& entity, entity_tag_name_t tag);
    void update() override;

    entity_tag_id_t createTag(const entity_tag_name_t& tagName);
    [[nodiscard]] entity_tag_id_t getTagId(
        const entity_tag_name_t& tagName) const;

    [[nodiscard]] bool exists(entity_id_t id) const;

    /*
template <typename T = Entity, typename... Args>
T *newEntity(Args &&...args) {
entity_id_t new_id{EntityManagerImpl::reserveId()};

PR_CORE_ASSERT(exists(new_id),
               "Attempted to create entity using existing id: {}",
               new_id);

auto new_entity{entity_unique_ptr(
    new T(new_id, args..uu.),

    [this](Entity *entity) { this->destroyEntity(entity); })};

auto *new_transform{newComponent<Transform>()};
new_entity->setComponent(new_transform);

entityMap_.emplace(new_id, std::move(new_entity));
return entityMap_[new_id].get();
};
    */

    entity_id_t reserveId();

   protected:
    EntityManagerImpl();
    ~EntityManagerImpl() override;

   private:
    void instantiateEntities();
    void collectGarbage();

    template <ComponentType T>
    std::vector<ComponentPtr<T>>* getComponentList() {
        auto it{componentDatabase_.find(ClassID<T>)};

        if (it == componentDatabase_.end()) {
            auto emplaced{
                componentDatabase_.emplace(ClassID<T>, ComponentList{})};
            it = componentDatabase_.find(ClassID<T>);
        }

        return reinterpret_cast<std::vector<ComponentPtr<T>>*>(&(it->second));
    }

    void destroyEntity(Entity* entity);

    ComponentDatabase componentDatabase_;

    struct Impl;
    Impl* impl_;

    // ComponentMap components_;
};
}  // namespace Presto
