#include "Presto/Modules/RenderingModule/RenderTypes.h"
#include "_Renderer.h"

#include <set>

namespace Presto {
    typedef uint32_t layer_id_t;

    class PRESTO_API RenderingManager : public Module {
       public:
        void Init() override;
        void Shutdown() override;

        void Update() override;

        layer_id_t addLayer(size_t pos = -1);
        void removeLayer(layer_id_t id);

        void addEntity(layer_id_t layer_index, Entity* ptr_entity);
        void removeEntity(Entity* ptr_entity);

       private:
        Renderer* _renderer;
        std::vector<RenderLayer> _renderLayers;
        std::set<Entity*> _boundEntities;

        bool hasLayer(layer_id_t index) const;
        RenderLayer& getLayer(layer_id_t id);
    };
}  // namespace Presto