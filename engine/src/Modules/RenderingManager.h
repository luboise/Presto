#pragma once

#include "Module.h"

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Assets/MaterialAsset.h"
#include "Presto/Objects/Components/CameraComponent.h"
#include "Presto/Rendering/PipelineBuilder.h"
#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Types/CoreTypes.h"

#include "Presto/Rendering/TextureTypes.h"
#include "Presto/Types/MaterialTypes.h"
#include "Rendering/MeshRegistrationData.h"

namespace Presto {

class GLFWAppWindow;

class CameraComponent;

class Renderer;
class Buffer;
class UniformBuffer;
class Pipeline;
class Texture;
struct ImportedMesh;

struct MeshData;

class Mesh;

struct Line;

using layer_id_t = PR_NUMERIC_ID;

struct AllocatedPipeline;

class PRESTO_API RenderingManager final : public Module<RenderingManager> {
    MODULE_FUNCTIONS(RenderingManager);

    friend bool ImageAsset::load();
    friend bool MaterialAsset::load();

   public:
    ~RenderingManager() override;

    static constexpr PR_NUMERIC_ID PR_MINIMUM_MATERIAL_KEY = 10;

    // TODO: Get rid of this and move it into the constructor
    static void init();

    // Loads the default Pipelines, default Materials for each pipeline and
    // default Textures
    void loadDefaults();

    void update() override;
    void clear();

    static void setRenderLibrary(RENDER_LIBRARY library);
    static void setWindow(GLFWAppWindow* window);

    [[nodiscard]] const PipelineStructure* getPipelineStructure(
        pipeline_id_t) const;

    void setMainCamera(const EntityPtr&);

    EntityPtr getMainCamera();
    ComponentPtr<CameraComponent> getDebugCamera();

    /**
     * @brief  Loads an imported mesh into the renderer, and registers it with
     * its pipeline.
     */
    Ptr<Mesh> loadMesh(MeshData meshData,
                       mesh_registration_id_t customId = PR_UNREGISTERED);

    /**
     * @brief  Unloads an existing Mesh from the renderer, and destroys it
     */
    void unloadMesh(Ptr<Mesh>&&);

    layer_id_t addLayer(size_t pos = -1);
    void removeLayer(layer_id_t id);

    PipelineBuilder& getPipelineBuilder();

    [[nodiscard]] Allocated<UniformBuffer> createUniformBuffer(
        Presto::size_t size);

    [[nodiscard]] Ptr<MaterialInstance> createMaterial(MaterialType type,
                                                       Presto::string name);
    [[nodiscard]] Ptr<MaterialInstance> findMaterial(
        const Presto::string& name);

    [[nodiscard]] Ptr<Texture2D> createTexture2D(const ImagePtr& image_ptr);
    [[nodiscard]] Ptr<Texture2D> createTexture2D(Presto::size_t width,
                                                 Presto::size_t height);

    Ptr<Texture> getDefaultTexture(const char* name);

    void drawLine(const Line&);

    void setUsingDebugCamera(bool isUsing);
    [[nodiscard]] bool& usingDebugCamera();

    Ptr<Texture> getTexture(texture_id_t);

    [[nodiscard]] VisualExtents framebufferSize() const;

    Allocated<MeshRegistrationData> allocateMeshRegistration(
        pipeline_id_t pipelineId, Presto::size_t vertexSize,
        Presto::size_t vertexCount, Presto::size_t indexCount);

    void drawFromAllocation(MeshRegistrationData&);

    void usePipeline(pipeline_id_t id);

   private:
    // Static vars
    static RENDER_LIBRARY _library;
    static GLFWAppWindow* _window;

    /*
// TODO: Make this take a reference instead and clean up the logic
void loadImageOnGpu(const ImagePtr&);
void loadImageOnGpu(ImageAsset&);
    */

    void setPipeline(pipeline_id_t, Pipeline pipeline);
    void setTexture(texture_id_t id, Texture texture);

    texture_id_t addTexture(const Presto::ImageData& image);
    void removeTexture(texture_id_t);

    PipelineStructure addPipeline(Pipeline&& pipeline,
                                  pipeline_id_t id = PR_PIPELINE_ANY);

    [[nodiscard]] AllocatedPipeline* getPipeline(pipeline_id_t id) const;

    void switchPipeline(AllocatedPipeline* allocatedPipeline);
    void switchPipeline(pipeline_id_t id);

    void switchMaterial(const MaterialPtr& material);

    [[nodiscard]] Ptr<Texture2D> createTexture2D(ImageData data,
                                                 texture_id_t id);
    [[nodiscard]] Ptr<Texture2D> createTexture2D(Presto::size_t width,
                                                 Presto::size_t height,
                                                 texture_id_t id);

    template <typename T>
    Allocated<MeshRegistrationData> createMeshRegistration(
        const std::vector<T>& vertices, IndexList& indices,
        pipeline_id_t pipelineId);

    explicit RenderingManager(RENDER_LIBRARY library, GLFWAppWindow* window);

    Allocated<Renderer> renderer_;

    struct Impl;
    Allocated<Impl> impl_;
};

}  // namespace Presto
