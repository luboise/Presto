#pragma once

#include <vector>
#include "Presto/Core/Constants.h"
#include "Presto/Types/ObjectTypes.h"

namespace Presto {
class Entity;

constexpr auto PRESTO_FIGURE_MAX_ENTITY_COUNT = 20;
using figure_size_t = PR_SIZE;

class Figure {
   public:
    explicit Figure(figure_size_t size);
    ~Figure();

    Entity& get(std::size_t index);

   private:
    std::vector<EntityPtr> entities_;
};
}  // namespace Presto
