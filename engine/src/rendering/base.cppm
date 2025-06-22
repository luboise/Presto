export module presto.internal.rendering:base;
export import presto.internal.rendering.types;

import presto.core.types;

export namespace Presto {

class GLFWAppWindow;
class CameraComponent;
class ImageData;
class UniformLayout;
struct ImportedMesh;

class Pipeline;
class PipelineBuilder;

class UniformBuffer;

struct PipelineStructure;

class Renderer : protected LazyCalculator {
   public:
    static Allocated<Renderer> create(RENDER_LIBRARY lib,
                                      GLFWAppWindow* window);

    virtual ~Renderer() = default;

    /**
     * Returns a builder that can be used to create new graphics
     * pipelines. The builder is determined by the renderer implementation, and
     * is assumed to be able to create new graphics pipelines by its own means.
     *
     * @brief Returns a builder used to create new graphics pipelines.
     * @return The new PipelineBuilder object.
     */
    virtual Allocated<PipelineBuilder> getPipelineBuilder() = 0;

    virtual Allocated<TextureFactory> getTextureFactory() = 0;
    // virtual Allocated<Texture> createTexture(Presto::Image image) = 0;

    virtual Allocated<Buffer> createBuffer(Buffer::BufferType type,
                                           Presto::size_t size) = 0;
    virtual Allocated<UniformBuffer> createUniformBuffer(
        Presto::size_t size) = 0;

    using AllocatedPipelineList = std::vector<Allocated<Pipeline>>;
    virtual AllocatedPipelineList createDefaultPipelines() = 0;

    virtual bool createMeshContext(MeshRegistrationData&,
                                   const PipelineStructure&) = 0;
    virtual void render(MeshRegistrationData&) = 0;

    void setWindow(GLFWAppWindow* window);

    void setExtents(VisualExtents extents);
    [[nodiscard]] VisualExtents getExtents() const { return extents_; }

    virtual void nextFrame() = 0;

    void setCameraData(CameraComponent& camera);
    void setCameraData(GlobalUniforms&&);

    void setObjectData(ObjectUniforms&&);

    void framebufferResized() { this->_framebufferResized = true; }

    using aspect_ratio_t = double;
    [[nodiscard]] aspect_ratio_t aspectRatio() const { return aspectRatio_; };
    void setAspectRatio(aspect_ratio_t ratio);

    /* [[nodiscard]] virtual std::vector<PipelineStructure>
       getPipelineStructures() const = 0;
                 */

   protected:
    Renderer() = default;

    GLFWAppWindow* _glfwWindow{nullptr};

    GlobalUniforms globalUniforms_{};
    ObjectUniforms objectUniforms_{};

    bool _framebufferResized{false};

   private:
    virtual void recalculateViewport() = 0;

    VisualExtents extents_{};
    aspect_ratio_t aspectRatio_{16.0 / 9.0};
};

}  // namespace Presto
