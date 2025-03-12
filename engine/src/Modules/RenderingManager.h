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
struct MeshRegistrationData;

struct Line;

using layer_id_t = PR_NUMERIC_ID;

class PRESTO_API RenderingManager final : public Module<RenderingManager> {
    MODULE_FUNCTIONS(RenderingManager);

    friend bool ImageAsset::load();
    friend bool MaterialAsset::load();

   public:
    ~RenderingManager() override;

    static constexpr PR_NUMERIC_ID PR_MINIMUM_MATERIAL_KEY = 10;

    // TODO: Get rid of this and move it into the constructor
    static void init();

    void update() override;
    void clear();

    static void setRenderLibrary(RENDER_LIBRARY library);
    static void setWindow(GLFWAppWindow* window);

    [[nodiscard]] const PipelineStructure* getPipelineStructure(
        pipeline_id_t) const;

    void setMainCamera(const Ptr<CameraComponent>&);

    CameraComponent& getMainCamera();
    CameraComponent& getDebugCamera();

    /**
     * Loads an imported mesh into the renderer, and registers it with its
     * pipeline.
     */

    mesh_registration_id_t loadMesh(
        MeshData meshData, mesh_registration_id_t customId = PR_UNREGISTERED);

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

    void drawLine(const Line&);

    void setUsingDebugCamera(bool isUsing);
    [[nodiscard]] bool& usingDebugCamera();

   private:
    // Static vars
    static RENDER_LIBRARY _library;
    static GLFWAppWindow* _window;

    /*
// TODO: Make this take a reference instead and clean up the logic
void loadImageOnGpu(const ImagePtr&);
void loadImageOnGpu(ImageAsset&);
    */

    void resizeFramebuffer() const;

    void setPipeline(pipeline_id_t, Pipeline pipeline);
    void setTexture(texture_id_t id, Texture texture);

    Texture* getTexture(texture_id_t);
    texture_id_t addTexture(const Presto::Image& image);
    void removeTexture(texture_id_t);

    PipelineStructure addPipeline(Pipeline&& pipeline,
                                  pipeline_id_t id = PR_PIPELINE_ANY);

    [[nodiscard]] Pipeline* getPipeline(pipeline_id_t id) const;

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
