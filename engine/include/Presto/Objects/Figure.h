#pragma once

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
        std::vector<Entity*> entities_;
    };
}  // namespace Presto
