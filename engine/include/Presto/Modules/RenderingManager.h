#pragma once

#include "Module.h"

#include "Presto/Components/Camera.h"
#include "Presto/Components/Renderable.h"

#include "Presto/Rendering/RenderLayer.h"

#include "Presto/Resources/ImageResource.h"

#include "Presto/Resources/MaterialResource.h"
#include "Presto/Resources/MeshResource.h"

#include "Presto/Runtime/GLFWAppWindow.h"
#include "Presto/Utils/Allocator.h"

namespace Presto {
    using layer_id_t = PR_NUMERIC_ID;

    class RenderLayer;

    class PRESTO_API RenderingManager : public Module<RenderingManager> {
        friend class Application;
        friend void ImageResource::load();
        friend void MaterialResource::load();

       public:
        static void init(Camera& defaultCamera);

        void update() override;
        void clear();

        static void shutdown();

        static void setRenderLibrary(RENDER_LIBRARY library);
        static void setWindow(GLFWAppWindow* window);

        void setCamera(Camera& newCam);
        inline Camera& getCamera() { return activeCamera_; };

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

        void loadImageOnGpu(ImageResource*);

        void resizeFramebuffer() const;

        // Member vars
        Camera& activeCamera_;

        explicit RenderingManager(RENDER_LIBRARY library, GLFWAppWindow* window,
                                  Camera& defaultCamera);

        Renderer* renderer_;

        std::vector<RenderLayer> _renderLayers;

        // Allocator<Mesh> _meshes;
        Allocator<Renderable> _renderables;

        bool hasLayer(layer_id_t index);
        RenderLayer& getLayer(layer_id_t id);
    };
}  // namespace Presto
