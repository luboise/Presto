#pragma once

#include "Presto/Types/CoreTypes.h"

#include "Entity.h"

namespace Presto {
using scene_name_t = PR_STRING_ID;

class Scene {
   public:
    static constexpr Scene* INVALID = nullptr;

    explicit Scene(scene_name_t name)
        : name_(std::move(name)), entityList_({}) {};

    void addEntity(EntityPtr entity) { entityList_.push_back(entity); };

    [[nodiscard]] std::vector<EntityPtr> getEntities() const;

    [[nodiscard]] scene_name_t getName() const { return name_; };

   private:
    scene_name_t name_;

    // TODO: Make sure scenes can't share the same entity
    std::vector<EntityPtr> entityList_;
};
}  // namespace Presto
