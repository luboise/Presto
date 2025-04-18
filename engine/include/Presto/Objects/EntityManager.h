#pragma once

#include "Presto/Core/ViewHandling.h"
#include "Presto/Objects/Component.h"
#include "Presto/Objects/Entity.h"

#include "Presto/Objects/Figure.h"

namespace Presto {
class PRESTO_API EntityManager {
    friend class Application;

    friend Figure::~Figure();

    using EntityMap = std::map<entity_id_t, EntityPtr>;

   public:
    static EntityManager& Get();

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

    entity_tag_id_t createTag(const entity_tag_name_t& tagName);
    [[nodiscard]] entity_tag_id_t getTagId(
        const entity_tag_name_t& tagName) const;

    [[nodiscard]] bool exists(entity_id_t id) const;

    /*
template <typename T = Entity, typename... Args>
T *newEntity(Args &&...args) {
entity_id_t new_id{EntityManager::reserveId()};

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

    // TODO: Consider making this private. It's not a huge deal either
    // way, and people can just choose which one they use.
    template <typename T, typename... Args>
    // TODO: Fix this concept
    // requires std::constructible_from<T, Args...>
    ComponentPtr<T> newComponent(Args&&... args) {
        std::vector<ComponentPtr<T>>* list{getComponentList<T>()};

        // std::unique_ptr<Component> new_component{new T};
        ComponentPtr<T> new_component{new T(std::forward<Args>(args)...)};

        component_id_t new_id{reserveId()};
        new_component->id_ = new_id;

        auto& emplaced{list->emplace_back(new_component)};

        return std::dynamic_pointer_cast<T>(emplaced);
    };

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

    entity_id_t reserveId();

    ComponentDatabase componentDatabase_;

    // ComponentMap components_;
};

}  // namespace Presto
