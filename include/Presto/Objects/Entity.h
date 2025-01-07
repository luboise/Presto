#pragma once

#include "Component.h"
// #include "Presto/Core/Constants.h"

#include <map>

namespace Presto {
    // Forward declaration
    class EntityManager;

    using ComponentMap = std::map<component_class_t, Component*>;
    using entity_id_t = PR_NUMERIC_ID;

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

        virtual ~Entity();

       private:
        explicit Entity(entity_id_t id);
        //        virtual ~Entity();

        entity_id_t id_{UNASSIGNED_ID};
        ComponentMap components_;
        glm::vec3 _position{0, 0, 0};
        glm::vec3 _rotation{0, 0, 0};
    };

    using entity_ptr = Entity*;
}  // namespace Presto
