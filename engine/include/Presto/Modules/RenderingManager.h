#pragma once

#include <map>

#include "Module.h"

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Assets/MaterialAsset.h"
#include "Presto/Core/Types.h"
#include "Presto/Rendering/PipelineBuilder.h"
#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/RenderTypes.h"

#include "Presto/Rendering/Textures.h"

namespace Presto {
class GLFWAppWindow;
class Renderer;
class CameraComponent;

class Buffer;
class UniformBuffer;

class Pipeline;
class Texture;

struct ImportedMesh;

using layer_id_t = PR_NUMERIC_ID;

class PRESTO_API RenderingManager final : public Module<RenderingManager> {
    MODULE_FUNCTIONS(RenderingManager);

    friend bool ImageAsset::load();
    friend bool MaterialAsset::load();

    using mesh_id_t = Presto::uint16_t;      // 65,000 unique meshes
    using material_id_t = Presto::uint16_t;  // 65,000 unique meshes
    using texture_id_t = Presto::uint32_t;   // 2 billion unique textures

    struct MeshRegistrationData {
        mesh_id_t id;
        Allocated<Buffer> vertices;
        Allocated<Buffer> indices;
    };

   public:
    static constexpr PR_NUMERIC_ID PR_MINIMUM_MATERIAL_KEY = 10;

    static void init(CameraComponent& defaultCamera);

    void update() override;
    void clear();

    static void setRenderLibrary(RENDER_LIBRARY library);
    static void setWindow(GLFWAppWindow* window);

    [[nodiscard]] PipelineStructure* getPipelineStructure(
        renderer_pipeline_id_t) const;

    void setCamera(CameraComponent& newCam);
    CameraComponent& getCamera() { return activeCamera_; };

    /**
     * Loads an imported mesh into the renderer. It must be registered with a
     * pipeline before it can be drawn.
     */
    renderer_mesh_id_t loadMesh(const ImportedMesh& mesh);

    layer_id_t addLayer(size_t pos = -1);
    void removeLayer(layer_id_t id);

    PipelineBuilder& getPipelineBuilder();

    Allocated<UniformBuffer> createUniformBuffer(Presto::size_t size);

    [[nodiscard]] Ptr<MaterialInstance> createMaterial(MaterialType type,
                                                       Presto::string name);
    [[nodiscard]] Ptr<MaterialInstance> getMaterial(const Presto::string& name);

    [[nodiscard]] Ptr<Texture2D> createTexture2D(const ImagePtr& image_ptr);

    [[nodiscard]] Ptr<Texture2D> createTexture2D(Presto::size_t width,
                                                 Presto::size_t height);

    /*
void AddRenderable(layer_id_t layer_index, Renderable*);

void RemoveRenderable(Renderable* ptr_renderable) {
_renderables.release(ptr_renderable);
};
    */

    // Mesh* NewMesh(const VertexList&, const IndexList&);
    // Renderable* NewRenderable(PrestoRenderableConstructorArgs);

    static constexpr renderer_pipeline_id_t ANY_PIPELINE = -1;

    renderer_mesh_id_t createMeshContext(const ImportedMesh&);

    /*
MeshContext* getMeshContext(renderer_mesh_id_t);
void destroyMeshContext(renderer_mesh_id_t);
    */

    /*
OpenGLPipeline* getPipeline(renderer_pipeline_id_t);
    */

    // TODO: Implement custom shaders/materials
    /*
     renderer_material_id_t addMaterial(const Presto::Image& image);
     void removeMaterial(renderer_material_id_t);
            */

   private:
    // Static vars
    static RENDER_LIBRARY _library;
    static GLFWAppWindow* _window;

    // TODO: Make this take a reference instead and clean up the logic
    void loadImageOnGpu(const ImagePtr&);
    void loadImageOnGpu(ImageAsset&);

    void resizeFramebuffer() const;

    void setPipeline(renderer_pipeline_id_t, Pipeline pipeline);
    void setTexture(renderer_texture_id_t id, Texture texture);

    Texture* getTexture(renderer_texture_id_t);
    renderer_texture_id_t addTexture(const Presto::Image& image);
    void removeTexture(renderer_texture_id_t);

    PipelineStructure addPipeline(Pipeline&& pipeline,
                                  renderer_pipeline_id_t id = ANY_PIPELINE);

    // Member vars
    CameraComponent& activeCamera_;

    explicit RenderingManager(RENDER_LIBRARY library, GLFWAppWindow* window,
                              CameraComponent& defaultCamera);

    Allocated<Renderer> renderer_;

    std::map<renderer_mesh_id_t, Allocated<MeshRegistrationData>>
        meshRegistrations_;

    struct Impl;
    Allocated<Impl> impl_;
};

}  // namespace Presto
