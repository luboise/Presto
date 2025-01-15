#pragma once

#include "Component.h"
#include "Presto/Core/Types.h"
// #include "Presto/Core/Constants.h"

#include <array>
#include <map>

namespace Presto {
    // Forward declaration
    class EntityManager;

    using ComponentMap = std::map<component_class_t, Component*>;

    using entity_id_t = PR_NUMERIC_ID;
    using entity_name_t = PR_STRING_ID;

    constexpr size_t MAX_TAG_COUNT = 20;
    using entity_tag_id_t = int8_t;
    using entity_tag_name_t = PR_STRING_ID;
    using entity_tag_map = std::array<bool, MAX_TAG_COUNT>;

    constexpr entity_tag_id_t INVALID_TAG_ID = -1;

    class PRESTO_API Entity {
        friend class EntityManager;

       public:
        static constexpr auto UNASSIGNED_ID{static_cast<entity_id_t>(-1)};

        [[nodiscard]] entity_id_t getId() const { return this->id_; };

        template <typename ComponentClass>
        void setComponent(ComponentClass* component_ptr) {
            component_class_t classID = CLASS_ID(ComponentClass);
            components_.emplace(classID, component_ptr);
        }

        template <typename ComponentClass>
        ComponentClass* getComponent() {
            component_class_t classID = CLASS_ID(ComponentClass);

            auto component_i = components_.find(classID);
            if (component_i == components_.end()) {
                return nullptr;
            }

            auto* x = component_i->second;
            return dynamic_cast<ComponentClass*>(x);
        }

        ComponentMap getComponents();

        void addTag(const entity_tag_name_t& tag);
        void addTag(entity_tag_id_t tag);

        [[nodiscard]] entity_name_t getName() const { return name_; }

        // TODO: Fix destruction of entities and make the entity manager clean
        // it up instead
        virtual ~Entity();

       private:
        explicit Entity(entity_id_t id, entity_name_t = "Entity");

        entity_name_t name_;
        entity_id_t id_{UNASSIGNED_ID};

        entity_tag_map tags_;

        ComponentMap components_;
        glm::vec3 _position{0, 0, 0};
        glm::vec3 _rotation{0, 0, 0};
    };

    using entity_ptr = Entity*;
}  // namespace Presto
