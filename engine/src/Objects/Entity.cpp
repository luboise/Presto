#include "Presto/Objects/Entity.h"
#include <memory>

#include "Presto/Components/ConductorComponent.h"

#include "Modules/EventManager.h"
#include "Presto/Components/Physics/RigidBodyComponent.h"
#include "Presto/Modules/EntityManager.h"
#include "Presto/Modules/PhysicsManager.h"

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

    Entity::ComponentMap& Entity::getComponents() { return components_; }

    void Entity::checkNewComponent(GenericComponentPtr componentPtr) {
        // auto* conductor_ptr{dynamic_cast<ConductorComponent*>(componentPtr)};
        auto conductor_ptr{
            std::dynamic_pointer_cast<ConductorComponent>(componentPtr)};

        if (conductor_ptr != nullptr) {
            EventManager::get().registerCallbacks(*this);
        }

        auto rigidbody_ptr{
            std::dynamic_pointer_cast<RigidBodyComponent>(componentPtr)};

        if (rigidbody_ptr != nullptr) {
            PhysicsManager::get().addPairing(
                {.entity = this, .body = rigidbody_ptr});
        }
    };

    std::vector<ComponentPtr<ConductorComponent>> Entity::getConductors() {
        auto data{getComponents() | std::views::values |
                  std::views::transform(
                      [](auto& val) -> ComponentPtr<ConductorComponent> {
                          return std::dynamic_pointer_cast<ConductorComponent>(
                              val);
                      }) |
                  std::views::filter(
                      [](const auto& val) -> bool { return val != nullptr; })};

        return {data.begin(), data.end()};
    }
}  // namespace Presto
