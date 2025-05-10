#pragma once

#include "Presto/Types/ObjectTypes.h"

namespace Presto {

class Entity;

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

}  // namespace Presto
