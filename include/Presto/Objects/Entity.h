#pragma once

#include "Presto/Component.h"
#include "glm/fwd.hpp"

#include <map>

namespace Presto {
    // Forward declaration
    class EntityManager;

    using ComponentMap = std::map<component_class_t, Component*>;
    using entity_id_t = uint32_t;

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
        ComponentClass& getComponent() {
            component_class_t classID = typeid(ComponentClass).hash_code();
            return _components[classID];
        }

        ComponentMap getComponents();

       protected:
        Entity();
        virtual ~Entity();

       private:
        void setId(entity_id_t);

        entity_id_t _id = -1;
        ComponentMap _components;
        glm::vec3 _position;
        glm::vec3 _rotation;
    };

    using entity_t = Entity*;
}  // namespace Presto
