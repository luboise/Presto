#pragma once

#include "Module.h"

#include "Presto/Components/Renderable.h"
#include "Presto/Rendering/RenderLayer.h"

#include "Presto/Rendering/Camera.h"

#include "Presto/Resources/MeshResource.h"
#include "Presto/Utils/Allocator.h"

namespace Presto {
    using layer_id_t = PR_NUMERIC_ID;

    class RenderLayer;

    class PRESTO_API RenderingManager : public Module<RenderingManager> {
       public:
        static void Init();

        void Update() override;
        void Clear();

        static void Shutdown();

        static void setRenderLibrary(RENDER_LIBRARY library);

        static void setWindow(GLFWAppWindow* window);

        inline Camera& getCamera() { return camera_; };
        void setCamera(Camera& newCam);

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
        // Renderable* NewRenderable(PrestoRenderableConstructorArgs);

       private:
        // Static vars
        static RENDER_LIBRARY _library;
        static GLFWAppWindow* _window;

        // Member vars
        Camera camera_;

        explicit RenderingManager(RENDER_LIBRARY library,
                                  GLFWAppWindow* window);

        Renderer* renderer_;

        std::vector<RenderLayer> _renderLayers;

        // Allocator<Mesh> _meshes;
        Allocator<Renderable> _renderables;

        bool hasLayer(layer_id_t index);
        RenderLayer& getLayer(layer_id_t id);
    };
}  // namespace Presto
