#include "Presto/Objects/Figure.h"
#include "Presto/Modules/EntityManager.h"

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

}  // namespace Presto
