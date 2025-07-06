export module presto.objects.entity:figure;

import std;

import presto.types.core;
import presto.objects.entity;

export namespace Pr {

constexpr auto PRESTO_FIGURE_MAX_ENTITY_COUNT = 20;
using figure_size_t = Pr::size_t;

class Figure {
   public:
    Figure(const Figure&) = delete;
    Figure& operator=(const Figure&) = delete;
    explicit Figure(figure_size_t size);

    Figure(Figure&&) noexcept;
    Figure& operator=(Figure&&) noexcept;

    ~Figure();

    Entity& getEntity(Pr::size_t index = 0);

   private:
    std::vector<EntityPtr> entities_;
};
}  // namespace Pr
