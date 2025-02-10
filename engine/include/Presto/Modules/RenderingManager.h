#pragma once

#include "Module.h"

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Assets/MaterialAsset.h"
#include "Presto/Assets/ModelAsset.h"
#include "Presto/Rendering/PipelineBuilder.h"

namespace Presto {
class GLFWAppWindow;
class Renderer;
class CameraComponent;

using layer_id_t = PR_NUMERIC_ID;

class PRESTO_API RenderingManager final : public Module<RenderingManager> {
    MODULE_FUNCTIONS(RenderingManager);

    friend void ImageAsset::load();
    friend void MaterialAsset::load();

   public:
    static void init(CameraComponent& defaultCamera);

    void update() override;
    void clear();

    static void shutdown();

    static void setRenderLibrary(RENDER_LIBRARY library);
    static void setWindow(GLFWAppWindow* window);

    void setCamera(CameraComponent& newCam);
    CameraComponent& getCamera() { return activeCamera_; };

    void loadMeshOnGpu(MeshAsset&);
    void loadModelOnGpu(ModelAsset&);

    layer_id_t addLayer(size_t pos = -1);
    void removeLayer(layer_id_t id);

    PipelineBuilder getPipelineBuilder();

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

    // TODO: Make this take a reference instead and clean up the logic
    void loadImageOnGpu(const ImagePtr&);
    void loadImageOnGpu(ImageAsset&);

    void resizeFramebuffer() const;

    // Member vars
    CameraComponent& activeCamera_;

    explicit RenderingManager(RENDER_LIBRARY library, GLFWAppWindow* window,
                              CameraComponent& defaultCamera);

    Allocated<Renderer> renderer_;

    // std::vector<RenderLayer> _renderLayers;

    // Allocator<Mesh> _meshes;
    // Allocator<Renderable> _renderables;

    // bool hasLayer(layer_id_t index);
    // RenderLayer& getLayer(layer_id_t id);
};
}  // namespace Presto
