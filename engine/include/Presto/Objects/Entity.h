#pragma once

#include "Component.h"
#include "PrestoCore/Core/Constants.h"
#include "glm/fwd.hpp"

#include <map>

namespace Presto {
    // Forward declaration
    class EntityManager;

    using ComponentMap = std::map<component_class_t, Component*>;
    using entity_id_t = PR_NUMERIC_ID;

    class PRESTO_API Entity {
        friend class EntityManager;

       public:
        [[nodiscard]] entity_id_t getId() const;

        template <typename ComponentClass>
        void setComponent(ComponentClass* component_ptr) {
            component_class_t classID = typeid(ComponentClass).hash_code();
            _components.emplace(classID, component_ptr);
        }

        template <typename ComponentClass>
        ComponentClass* getComponent() {
            component_class_t classID = typeid(ComponentClass).hash_code();
            return dynamic_cast<ComponentClass*>(_components[classID]);
        }

        ComponentMap getComponents();

       private:
        explicit Entity(entity_id_t id);
        virtual ~Entity();

        entity_id_t _id;
        ComponentMap _components;
        glm::vec3 _position;
        glm::vec3 _rotation;
    };

    using entity_ptr = Entity*;
}  // namespace Presto
