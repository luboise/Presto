export module presto.objects.entity:figure;

import presto.core.types;

export namespace Presto {
class Entity;

constexpr auto PRESTO_FIGURE_MAX_ENTITY_COUNT = 20;
using figure_size_t = PR_SIZE;

class Figure {
   public:
    Figure(const Figure&) = delete;
    Figure& operator=(const Figure&) = delete;
    explicit Figure(figure_size_t size);

    Figure(Figure&&) noexcept;
    Figure& operator=(Figure&&) noexcept;

    ~Figure();

    Entity& getEntity(std::size_t index = 0);

   private:
    std::vector<EntityPtr> entities_;
};
}  // namespace Presto
