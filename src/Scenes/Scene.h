#include "Presto/Objects/Entity.h"

namespace Presto {
    using scene_id_t = std::string;

    class Scene {
       public:
        static constexpr scene_id_t INVALID = "INVALID_SCENE";

        inline void addEntity(entity_t entity) { _entities.push_back(entity); };

        [[nodiscard]] inline scene_id_t getId() { return _name; }

       private:
        scene_id_t _name;
        std::vector<entity_t> _entities;
    };
}  // namespace Presto
