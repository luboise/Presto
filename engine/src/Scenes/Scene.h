#include "Presto/Objects/Entity.h"

namespace Presto {
    using scene_id_t = std::string;

    class Scene {
       public:
        static constexpr scene_id_t INVALID = "INVALID_SCENE";

        inline void addEntity(entity_t entity) { _entities.push_back(entity); };

       private:
        scene_id_t _name;
        std::vector<entity_t> _entities;
    };
}  // namespace Presto
