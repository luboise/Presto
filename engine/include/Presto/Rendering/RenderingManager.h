#pragma once

// #include "Presto/Components/Renderable.h"
// #include "Presto/Components/Renderable/Mesh.h"
#include "Presto/Module.h"
#include "Presto/Rendering/Camera.h"
#include "Presto/Rendering/RenderLayer.h"
// #include "PrestoCore/Core/Constants.h"
// #include "PrestoCore/Rendering/Renderer.h"

#include "Presto/Tools/Allocator.h"

#include "Presto/Resources/MeshResource.h"

class RenderableProps;

namespace Presto {
    using layer_id_t = PR_NUMERIC_ID;

    class PRESTO_API RenderingManager : public Module<RenderingManager> {
       public:
        static void Init();
        void Update() override;
        static void Shutdown();

        static void setRenderLibrary(RENDER_LIBRARY library) {
            PR_CORE_ASSERT(!RenderingManager::IsInitialised(),
                           "Unable to set render library while the renderer is "
                           "already initialised.");

            RenderingManager::_library = library;
        }

        static void setWindow(GLFWAppWindow* window) {
            PR_CORE_ASSERT(!RenderingManager::IsInitialised(),
                           "Unable to set window surface while the renderer is "
                           "already initialised.");

            RenderingManager::_window = window;
        }

        void setCamera(Camera& newCam) {
            PR_CORE_ASSERT(RenderingManager::IsInitialised(),
                           "Unable to set camera when the RenderingManager is "
                           "uninitialised.")
            currentCamera_ = &newCam;
        }

        void renderFrame();

        void loadMeshOnGpu(MeshResource&);

        RenderingManager(const RenderingManager&) = delete;
        RenderingManager(RenderingManager&&) = delete;
        RenderingManager& operator=(const RenderingManager&) = delete;
        RenderingManager& operator=(RenderingManager&&) = delete;

        layer_id_t addLayer(size_t pos = -1);
        void removeLayer(layer_id_t id);

        /*
void AddRenderable(layer_id_t layer_index, Renderable*);

void RemoveRenderable(Renderable* ptr_renderable) {
    _renderables.release(ptr_renderable);
};
        */

        // Mesh* NewMesh(const VertexList&, const IndexList&);
        // RenderableProps* NewRenderableProps();
        // Renderable* NewRenderable(PrestoRenderableConstructorArgs);

       private:
        // Static vars
        static RENDER_LIBRARY _library;
        static GLFWAppWindow* _window;

        // Member vars
        Camera* currentCamera_ = nullptr;

        explicit RenderingManager(RENDER_LIBRARY library,
                                  GLFWAppWindow* window);

        Renderer* renderer_;

        std::vector<RenderLayer> _renderLayers;

        // Allocator<Mesh> _meshes;
        Allocator<RenderableProps> _renderProps;
        Allocator<Renderable> _renderables;

        bool hasLayer(layer_id_t index);
        RenderLayer& getLayer(layer_id_t id);
    };
}  // namespace Presto
