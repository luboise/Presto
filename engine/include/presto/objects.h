#include <utility>

#include "presto/objects/entity.h"  // IWYU pragma: export

namespace Presto {

[[nodiscard]] EntityRef NewLooseEntity();

[[nodiscard]] EntityPtr NewEntity(Presto::vec3 pos = {});

/**
 * @brief  Returns a new entity which the consumer must keep track of and
 * destroy themselves. It will still handle its own rendering and component
 * calls, but its destruction is bound by the user.
 */
[[nodiscard]] EntityOwner NewOwnedEntity(Presto::vec3 pos = {});

/**
 * @brief  Gets a reference to the main camera. This is whats used to generate
 * the view of the game world.
 */
CameraComponent& GetDefaultCamera();

void SetDefaultCameraConductor(const ComponentPtr<ConductorComponent>&);

/**
 * @brief  Creates a new component of any type. The arguments given must
 * match the arguments of the private constructor of that component.
 */
template <DerivedFrom<Component> T, typename... Args>
[[nodiscard]] ComponentPtr<T> NewComponent(Args... args) {
    return EntityManager::Get().newComponent<T>(args...);
};

// Functions that create components
namespace CreateComponent {

template <DerivedFrom<ConductorComponent> T, typename... Args>
ComponentPtr<T> Conductor(Args... args) {
    return EntityManager::Get().newComponent<T>(args...);
};

}  // namespace CreateComponent

using namespace std::ranges;
using namespace std::views;

// Used by EntityManager.h
using ComponentFilter = std::function<bool(const GenericComponentPtr&)>;

using ComponentList = std::vector<GenericComponentPtr>;
using ComponentDatabase = std::map<class_id_t, ComponentList>;
using ComponentSearchResults =
    filter_view<all_t<join_view<elements_view<ref_view<ComponentDatabase>, 1>>>,
                ComponentFilter>;

class PRESTO_API EntityManager {
    friend class Application;

    friend Figure::~Figure();

    using EntityMap = std::map<entity_id_t, EntityPtr>;

   public:
    static EntityManager& Get();

    using ComponentMap = std::map<component_id_t, GenericComponentPtr>;

    [[nodiscard]] EntityPtr newEntity(const entity_name_t& name = "Entity");
    std::vector<EntityPtr> newEntities(Presto::size_t count);

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

class EntityOwner {
   public:
    EntityOwner();

    ~EntityOwner();

    EntityOwner(const EntityOwner&) = delete;
    EntityOwner& operator=(const EntityOwner&) = delete;

    EntityOwner(EntityOwner&&) noexcept;
    EntityOwner& operator=(EntityOwner&&) noexcept;

    Entity* operator->();

   private:
    EntityPtr entity_{nullptr};
};

struct TransformData {
    Presto::vec3 position;
    Quaternion rotation;

    TransformData();
    explicit TransformData(Presto::vec3 position, Quaternion rotation = {});
    TransformData(Presto::vec3 position, Presto::vec3 rotation);

    Presto::vec3 scale{1, 1, 1};

    [[nodiscard]] Presto::mat4 asModelMat() const;
    [[nodiscard]] Presto::mat4 asViewMat() const;

    [[nodiscard]] Presto::vec3 forwards() const;
    [[nodiscard]] Presto::vec3 backwards() const;

    [[nodiscard]] Presto::vec3 leftwards() const;
    [[nodiscard]] Presto::vec3 rightwards() const;

    [[nodiscard]] Presto::vec3 upwards() const;
    [[nodiscard]] Presto::vec3 downwards() const;

    TransformData& addRotation(Presto::vec3);
    TransformData& addRotation(Presto::Quaternion);

    TransformData& addTranslation(Presto::vec3);
    TransformData& scaleBy(Presto::vec3);
};

}  // namespace Presto
