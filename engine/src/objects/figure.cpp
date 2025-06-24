#include "presto/objects/entity.h"
import :figure;
#include <utility>

namespace Pr {
Figure::Figure(figure_size_t size) {
    PR_CORE_ASSERT(
        size > 0 && size < PRESTO_FIGURE_MAX_ENTITY_COUNT,
        std::format("Figure entity count is out of valid range [1, {}]",
                    PRESTO_FIGURE_MAX_ENTITY_COUNT));

    entities_ = EntityManagerImpl::get().newEntities(size);
}

Figure::Figure(Figure&& other) noexcept
    : entities_(std::move(other.entities_)) {
    other.entities_.clear();
};

Figure& Figure::operator=(Figure&& other) noexcept {
    this->entities_ = std::move(other.entities_);
    other.entities_.clear();

    return *this;
};

Figure::~Figure() {
    for (auto& ptr : entities_) {
        ptr->destroy();
    }
}

Entity& Figure::getEntity(Pr::size_t index) {
    PR_ASSERT(index < entities_.size(),
              std::format("Index out of acccepted bounds [0, {}]",
                          entities_.size() - 1));
    return *entities_[index];
};

}  // namespace Pr
