#include <utility>

#include "Presto/Objects/Entity.h"

#include "Presto/Modules/EntityManager.h"
#include "Presto/Runtime.h"

using Presto::ObjectCreatedEvent;

namespace Presto {
    Entity::Entity(entity_id_t id, entity_name_t name)
        : name_(std::move(name)), id_(id) {
        ObjectCreatedEvent(this);
    }

    void Entity::addTag(const entity_tag_name_t& tagName) {
        entity_tag_id_t tag_id{EntityManager::get().getTagId(tagName)};
        PR_ASSERT(tag_id != INVALID_TAG_ID,
                  std::format("Unable to get tag id for name {}. Has the tag "
                              "been created yet?",
                              tagName));

        addTag(tag_id);
    }

    void Entity::addTag(entity_tag_id_t tag) { tags_[tag] = true; };

    Entity::~Entity() = default;

    ComponentMap Entity::getComponents() { return components_; }
}  // namespace Presto
