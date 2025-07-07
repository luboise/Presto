module;
#include "presto/platform.h"

export module presto.objects.base;

import presto.core;
import presto.core.concepts;
import presto.types;

import std;

using namespace std::ranges;
using namespace std::views;

export namespace Pr {

using component_id_t = PR_NUMERIC_ID;

class Component;

template <typename T>
concept ComponentType = std::derived_from<T, Component>;

template <ComponentType T>
using ComponentPtr = Ptr<T>;

template <ComponentType T>
using ComponentRef = Ref<T>;

using GenericComponentPtr = ComponentPtr<Component>;

using class_id_t = Pr::size_t;
template <typename T>
const class_id_t ClassID = typeid(T).hash_code();

// Used by EntityManager
using ComponentFilter = std::function<bool(const GenericComponentPtr&)>;

using ComponentList = std::vector<GenericComponentPtr>;
using ComponentDatabase = std::map<class_id_t, ComponentList>;
using ComponentSearchResults =
    filter_view<all_t<join_view<elements_view<ref_view<ComponentDatabase>, 1>>>,
                ComponentFilter>;

class PRESTO_API Component {
    friend class EntityManager;

    using ComponentIDBit = Pr::uint32_t;

    static constexpr auto UNASSIGNED_ID{static_cast<component_id_t>(-1)};

   public:
    // [[nodiscard]] component_class_t getClassID() const {};

    template <ComponentType SubClass>
    bool isOfType() {
        return dynamic_cast<SubClass*>(this) != nullptr;
    }

    [[nodiscard]] bool enteredScene() const { return enteredScene_; }

    [[nodiscard]] component_id_t id() const { return this->id_; };

    Component(const Component&) = default;
    Component(Component&&) = delete;
    Component& operator=(const Component&) = default;
    Component& operator=(Component&&) = delete;

    /*
template <ComponentType SubClass>
ComponentPtr<SubClass> as() {
return ComponentPtr<SubClass>{dynamic_cast<SubClass*>(this)};
}
    */

   protected:
    Component() = default;
    virtual ~Component() = default;

   private:
    virtual void onEnterScene() {};

    bool enteredScene_{false};

    component_id_t id_{UNASSIGNED_ID};
};

template <typename T>
    requires DerivedFrom<T, Component, Strictness::STRICTLY_DERIVED>
struct Subcomponent {
    using super_t = T;
};

class PRESTO_API Entity {
    // friend class EntityManagerImpl;

   public:
    Entity() = delete;
    Entity(const Entity&) = delete;
    Entity(Entity&&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;

    using ComponentMap = std::map<class_id_t, ComponentPtr<Component>>;

    static constexpr auto UNASSIGNED_ID{static_cast<entity_id_t>(-1)};

    [[nodiscard]] entity_id_t id() const { return this->id_; };

    void destroy();

    // TODO: Move component ID into the component class if possible
    // (Commponent itself cannot be templated!)
    template <ComponentType ComponentClass>
    void setComponent(ComponentPtr<ComponentClass> component_ptr) {
        class_id_t id{ClassID<ComponentClass>};
        components_.emplace(id, component_ptr);

        checkNewComponent(component_ptr);
    }

    template <ComponentType ComponentClass>
    ComponentPtr<ComponentClass> getComponent() {
        auto id{ClassID<ComponentClass>};

        auto component_it{components_.find(id)};
        if (component_it == components_.end()) {
            return nullptr;
        }

        return {
            std::dynamic_pointer_cast<ComponentClass>(component_it->second)};
    }

    ComponentMap& components();

    void addTag(const entity_tag_name_t& tag);
    void addTag(entity_tag_id_t tag);

    [[nodiscard]] entity_name_t getName() const { return name_; }

    // TODO: Fix destruction of entities and make the entity manager clean
    // it up instead
    virtual ~Entity();

    // TODO: Make this not public (hotfix to get it working)
    void checkNewComponent(GenericComponentPtr componentPtr);

   protected:
    explicit Entity(entity_id_t id, entity_name_t = "Entity");

   private:
    entity_name_t name_;
    entity_id_t id_{UNASSIGNED_ID};

    entity_tag_map tags_{};

    ComponentMap components_;
};

using EntityPtr = Ptr<Entity>;
using EntityRef = Ref<Entity>;

class PRESTO_API EntityManager {
    using EntityMap = std::map<entity_id_t, EntityPtr>;

   public:
    static EntityManager& Get();

    using ComponentMap = std::map<component_id_t, GenericComponentPtr>;

    [[nodiscard]] EntityPtr newEntity(const entity_name_t& name = "Entity");
    std::vector<EntityPtr> newEntities(Pr::size_t count);

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

   protected:
    void instantiateEntities();
    void update();

   private:
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
};
}  // namespace Pr
