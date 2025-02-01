#include "Presto/Objects/Figure.h"

#include "Presto/Modules/EntityManager.h"
#include "Presto/Objects/Entity.h"

namespace Presto {
    Figure::Figure(figure_size_t size) {
        PR_CORE_ASSERT(
            size > 0 && size < PRESTO_FIGURE_MAX_ENTITY_COUNT,
            std::format("Figure entity count is out of valid range [1, {}]",
                        PRESTO_FIGURE_MAX_ENTITY_COUNT));

        entities_ = EntityManager::get().newEntities(size);
    }

    Figure::~Figure() {
        for (auto* ptr : entities_) {
            EntityManager::get().destroyEntity(ptr);
        }
    }

    Entity& Figure::get(std::size_t index) {
        PR_ASSERT(index < entities_.size(),
                  std::format("Index out of acccepted bounds [1, {}]",
                              entities_.size()));
        return *entities_[index];
    };
}  // namespace Presto
