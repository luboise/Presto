#include <utility>

#include "Presto/Objects/Entity.h"

#include "Presto/Runtime.h"

using Presto::ObjectCreatedEvent;

namespace Presto {
    Entity::Entity(entity_id_t id, entity_name_t name)
        : name_(std::move(name)), id_(id) {
        ObjectCreatedEvent(this);
    }

    Entity::~Entity() = default;

    ComponentMap Entity::getComponents() { return components_; }
}  // namespace Presto
