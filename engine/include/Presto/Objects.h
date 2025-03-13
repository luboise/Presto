#pragma once

#include "Presto/Core.h"

#include "Presto/Objects/Components.h"     // IWYU pragma: export
#include "Presto/Objects/Entity.h"         // IWYU pragma: export
#include "Presto/Objects/EntityManager.h"  // IWYU pragma: export

#include "Presto/Types/ObjectTypes.h"  // IWYU pragma: export

namespace Presto {

[[nodiscard]] EntityRef NewEntity();

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
ComponentPtr<T> NewComponent(Args... args) {
    return EntityManager::Get().newComponent<T>(args...);
};

// Functions that create components
namespace CreateComponent {
ComponentPtr<ModelComponent> Model(const ModelPtr& ptr);

template <DerivedFrom<ConductorComponent> T, typename... Args>
ComponentPtr<T> Conductor(Args... args) {
    return EntityManager::Get().newComponent<T>(args...);
};

}  // namespace CreateComponent

}  // namespace Presto
