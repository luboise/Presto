#pragma once

#include "Presto/Constants.h"

#include "Presto/Components/Renderable.h"
#include "Presto/Module.h"
#include "Presto/Rendering/Mesh.h"
#include "Presto/Rendering/Renderer.h"

#include "Presto/Rendering/Types/RenderLayer.h"

#include "Presto/Tools/Allocator.h"

class RenderableProps;

namespace Presto {
    using layer_id_t = uint32_t;

    class PRESTO_API RenderingManager : public Module<RenderingManager> {
       public:
        static void Init();

        void Update() override;
        void Flush();

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

        static void setCamera(Camera& newCam) {
            PR_CORE_ASSERT(RenderingManager::IsInitialised(),
                           "Unable to set camera when the RenderingManager is "
                           "uninitialised.")
            RenderingManager::Get()._renderer->setCamera(newCam);
        }

        layer_id_t AddLayer(size_t pos = -1);
        void RemoveLayer(layer_id_t id);

        void AddRenderable(layer_id_t layer_index, Renderable*);

        void RemoveRenderable(Renderable* ptr_renderable) {
            _renderables.release(ptr_renderable);
        };

        Mesh* NewMesh(const VertexList&, const IndexList&);
        RenderableProps* NewRenderableProps();
        Renderable* NewRenderable(PrestoRenderableConstructorArgs);

        RenderingManager(const RenderingManager&) = delete;
        RenderingManager(RenderingManager&&) = delete;
        RenderingManager& operator=(const RenderingManager&) = delete;
        RenderingManager& operator=(RenderingManager&&) = delete;

       private:
        static RENDER_LIBRARY _library;
        static GLFWAppWindow* _window;

        explicit RenderingManager(RENDER_LIBRARY library,
                                  GLFWAppWindow* window);

        Renderer* _renderer;

        std::vector<RenderLayer> _renderLayers;

        Allocator<Mesh> _meshes;
        Allocator<RenderableProps> _renderProps;
        Allocator<Renderable> _renderables;

        bool hasLayer(layer_id_t index);
        RenderLayer& getLayer(layer_id_t id);
    };
}  // namespace Presto
