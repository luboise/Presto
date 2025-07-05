module;
#include "presto/module.h"

export module presto.internal.managers.rendering_manager;

import presto.core.constants;
import presto.core.manager;
import presto.core.types;

import presto.assets.image;
import presto.assets.material;

import presto.internal.rendering;

export namespace Pr {

class MaterialInstance;

class RenderingManager final : public Module<RenderingManager> {
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

    PipelineBuilder& getPipelineBuilder();

    [[nodiscard]] Allocated<UniformBuffer> createUniformBuffer(Pr::size_t size);

    [[nodiscard]] Ptr<MaterialInstance> createMaterial(MaterialType type,
                                                       Pr::string name);
    [[nodiscard]] Ptr<MaterialInstance> findMaterial(const Pr::string& name);

    [[nodiscard]] Ptr<Texture2D> createTexture2D(
        const Pr::Ptr<Pr::ImageAsset>& image_ptr);
    [[nodiscard]] Ptr<Texture2D> createTexture2D(Pr::size_t width,
                                                 Pr::size_t height);

    Ptr<Texture> getDefaultTexture(const char* name);

    void drawLine(const Line&);

    void setUsingDebugCamera(bool isUsing);
    [[nodiscard]] bool& usingDebugCamera();

    Ptr<Texture> getTexture(texture_id_t);

    [[nodiscard]] VisualExtents framebufferSize() const;

    Allocated<MeshRegistrationData> allocateMeshRegistration(
        pipeline_id_t pipelineId, Pr::size_t vertexSize, Pr::size_t vertexCount,
        Pr::size_t indexCount);

    void drawFromAllocation(MeshRegistrationData&);

    void usePipeline(pipeline_id_t id);

   private:
    // Static vars
    static RENDER_LIBRARY _library;
    static GLFWAppWindow* _window;

    /*
// TODO: Make this take a reference instead and clean up the logic
void loadImageOnGpu(const Pr::Ptr<Pr::ImageAsset>&);
void loadImageOnGpu(ImageAsset&);
    */

    void setPipeline(pipeline_id_t, Pipeline pipeline);
    void setTexture(texture_id_t id, Texture texture);

    texture_id_t addTexture(const Pr::ImageData& image);
    void removeTexture(texture_id_t);

    PipelineStructure addPipeline(Pipeline&& pipeline,
                                  pipeline_id_t id = PR_PIPELINE_ANY);

    [[nodiscard]] AllocatedPipeline* getPipeline(pipeline_id_t id) const;

    void switchPipeline(AllocatedPipeline* allocatedPipeline);
    void switchPipeline(pipeline_id_t id);

    void switchMaterial(const Pr::Ptr<Pr::MaterialInstance>& material);

    [[nodiscard]] Ptr<Texture2D> createTexture2D(ImageData data,
                                                 texture_id_t id);
    [[nodiscard]] Ptr<Texture2D> createTexture2D(Pr::size_t width,
                                                 Pr::size_t height,
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

class MaterialInstance {
    friend class MaterialAsset;
    friend class RenderingManager;

   public:
    explicit MaterialInstance(const Pr::Ptr<Pr::MaterialAsset>&);
    ~MaterialInstance();

    MaterialInstance& setName(Pr::string newName);
    [[nodiscard]] Pr::string name() const;

    [[nodiscard]] const UniformLayout& getUniformStructure() const;

    [[nodiscard]] pipeline_id_t getPipelineId() const;

    void setFromImport(const ImportedMaterial& imported_material,
                       std::vector<Pr::Ptr<Pr::Texture>>& texturePtrs);

   private:
    void bindTo(Pipeline&) const;

    struct PropertyDetails {
        UniformBinding binding;
        // Data index is the index of the uniform block for block variables, and
        // the index of the data for regular uniform variables
        Pr::size_t data_index{};
    };

    PropertyDetails* getBinding(const Pr::string& name);
    UniformBuffer& getUniformBuffer(Pr::size_t index);
    ErasedBytes& getUniformDataStore(Pr::size_t index);

    struct Impl;
    Allocated<Impl> impl_;
};

template <>
MaterialInstance& MaterialInstance::setProperty(Pr::string name,
                                                const Ptr<Texture>& data);

class MaterialInstanceImpl : public MaterialInstance {
   public:

   private:
};

}  // namespace Pr
