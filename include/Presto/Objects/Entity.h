#pragma once

#include "Presto/Component.h"
#include "glm/fwd.hpp"

#include <map>

namespace Presto {
    // Forward declaration
    class EntityManager;

    typedef std::map<component_class_t, Component*> ComponentMap;
    typedef uint32_t entity_id_t;

    class PRESTO_API Entity {
        friend class EntityManager;

       public:
        entity_id_t getId() const;

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

       private:
        Entity(entity_id_t id);
        virtual ~Entity();

        entity_id_t _id;
        ComponentMap _components;
        glm::vec3 _position;
        glm::vec3 _rotation;
    };

    typedef Entity* entity_t;
}  // namespace Presto
