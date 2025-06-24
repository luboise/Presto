#include "presto/handles/object_handles.h"
#include "presto/objects/entity.h"  // IWYU pragma: export

#include "presto/objects/entity_manager.h"

#include "presto/aliases/object_aliases.h"

namespace Pr {
class EntityOwner;

[[nodiscard]] EntityRef NewLooseEntity();

[[nodiscard]] EntityPtr NewEntity(Pr::vec3 pos = {});

/**
 * @brief  Returns a new entity which the consumer must keep track of and
 * destroy themselves. It will still handle its own rendering and component
 * calls, but its destruction is bound by the user.
 */
[[nodiscard]] EntityOwner NewOwnedEntity(Pr::vec3 pos = {});

/**
 * @brief  Gets a reference to the main camera. This is whats used to generate
 * the view of the game world.
 */
CameraComponent& GetDefaultCamera();

void SetDefaultCameraConductor(const Ptr<Conductor>&);

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

class EntityOwner {
   public:
    EntityOwner();

    ~EntityOwner();

    EntityOwner(const EntityOwner&) = delete;
    EntityOwner& operator=(const EntityOwner&) = delete;

    EntityOwner(EntityOwner&&) noexcept;
    EntityOwner& operator=(EntityOwner&&) noexcept;

    Entity* operator->();

   private:
    EntityPtr entity_{nullptr};
};

}  // namespace Pr
