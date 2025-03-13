#pragma once

#include "Component.h"

#include "Presto/Objects/Components/ConductorComponent.h"

#include <map>
#include "Presto/Types/CoreTypes.h"

namespace Presto {
// Forward declaration
class EntityManager;

using entity_id_t = PR_NUMERIC_ID;
using entity_name_t = PR_STRING_ID;

constexpr size_t MAX_TAG_COUNT = 20;
using entity_tag_id_t = int8_t;
using entity_tag_name_t = PR_STRING_ID;
using entity_tag_map = std::array<bool, MAX_TAG_COUNT>;

constexpr entity_tag_id_t INVALID_TAG_ID = -1;

class PRESTO_API Entity {
    friend class EntityManagerImpl;
    friend class EventManager;

   public:
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

    [[nodiscard]] std::vector<ComponentPtr<ConductorComponent>> getConductors();

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

    ComponentMap& getComponents();

    void addTag(const entity_tag_name_t& tag);
    void addTag(entity_tag_id_t tag);

    [[nodiscard]] entity_name_t getName() const { return name_; }

    // TODO: Fix destruction of entities and make the entity manager clean
    // it up instead
    virtual ~Entity();

    // TODO: Make this not public (hotfix to get it working)
    void checkNewComponent(GenericComponentPtr componentPtr);

   private:
    explicit Entity(entity_id_t id, entity_name_t = "Entity");

    entity_name_t name_;
    entity_id_t id_{UNASSIGNED_ID};

    entity_tag_map tags_{};

    ComponentMap components_;
    glm::vec3 _position{0, 0, 0};
    glm::vec3 _rotation{0, 0, 0};
};

}  // namespace Presto
