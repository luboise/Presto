#include "Presto/Modules/RenderingModule/RenderTypes.h"
#include "Presto/Window.h"
#include "_Renderer.h"

#include <set>

#include "GLFW/glfw3.h"

namespace Presto {
    typedef uint32_t layer_id_t;

    class PRESTO_API RenderingManager : public Module {
        MODULE_FUNCTIONS();

       public:
        static void F_INIT(Renderer::RENDER_LIBRARY library,
                           GLFWwindow* windowPtr);

        static layer_id_t addLayer(size_t pos = -1);
        static void removeLayer(layer_id_t id);

        static void addEntity(layer_id_t layer_index, Entity* ptr_entity);
        static void removeEntity(Entity* ptr_entity);

       private:
        RenderingManager();

        inline static Renderer* _renderer;
        inline static GLFWwindow* _window;

        inline static std::vector<RenderLayer> _renderLayers;
        inline static std::set<Entity*> _boundEntities;

        inline static bool hasLayer(layer_id_t index);
        inline static RenderLayer& getLayer(layer_id_t id);
    };
}  // namespace Presto