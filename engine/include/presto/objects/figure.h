#include "presto/core/types.h"

namespace Presto {
class Entity;

constexpr auto PRESTO_FIGURE_MAX_ENTITY_COUNT = 20;
using figure_size_t = Presto::size_t;

class Figure {
   public:
    Figure(const Figure&) = delete;
    Figure& operator=(const Figure&) = delete;
    explicit Figure(figure_size_t size);

    Figure(Figure&&) noexcept;
    Figure& operator=(Figure&&) noexcept;

    ~Figure();

    Entity& getEntity(Presto::size_t index = 0);

   private:
    std::vector<EntityPtr> entities_;
};
}  // namespace Presto
