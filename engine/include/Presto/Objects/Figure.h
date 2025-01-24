#pragma once

#include "Presto/Objects.h"

namespace Presto {
    constexpr auto PRESTO_FIGURE_MAX_ENTITY_COUNT = 20;
    using figure_size_t = PR_SIZE;

    class Figure {
       public:
        explicit Figure(figure_size_t size);
        ~Figure();

        Entity& get(std::size_t index) {
            PR_ASSERT(index < entities_.size(),
                      std::format("Index out of acccepted bounds [1, {}]",
                                  entities_.size()));
            return *entities_[index];
        };

       private:
        std::vector<Entity*> entities_;
    };
}  // namespace Presto
