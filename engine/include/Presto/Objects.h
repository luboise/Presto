#pragma once

#include "Presto/Core.h"

#include "Presto/Objects/Components.h"     // IWYU pragma: export
#include "Presto/Objects/Entity.h"         // IWYU pragma: export
#include "Presto/Objects/EntityManager.h"  // IWYU pragma: export

#include "Presto/Objects/EntityOwner.h"
#include "Presto/Types/ObjectTypes.h"  // IWYU pragma: export

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

}  // namespace Presto
