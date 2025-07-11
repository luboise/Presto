module;
#include "presto/module.h"

export module presto.internal.managers.entity_manager_impl;

import std;

import presto.core.concepts;
import presto.internal.managers.manager;
import presto.objects;

export namespace Pr {
class PRESTO_API EntityManagerImpl final : public Module<EntityManagerImpl>,
                                           public EntityManager {
    MODULE_FUNCTIONS(EntityManagerImpl);

    // friend class Application;
    friend class Module;

    using EntityMap = std::map<entity_id_t, EntityPtr>;

   public:
    using ComponentMap = std::map<component_id_t, GenericComponentPtr>;

    EntityPtr getEntityByID(entity_id_t id);

    ComponentSearchResults findComponentsWhere(const ComponentFilter& =
                                                   [](auto&) { return true; });

    template <ComponentType T>
        requires DerivedFrom<T, Component, Strictness::STRICTLY_DERIVED>
    [[nodiscard]] std::vector<ComponentPtr<T>>& findComponentsByType() {
        return *getComponentList<T>();
    }

    void addTagToEntity(Entity& entity, entity_tag_name_t tag);
    void update() override;

    entity_tag_id_t createTag(const entity_tag_name_t& tagName);
    [[nodiscard]] entity_tag_id_t getTagId(
        const entity_tag_name_t& tagName) const;

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
};

}  // namespace Pr
