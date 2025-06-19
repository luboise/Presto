export module presto.internal.rendering:base;
export import :types;

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

class Buffer {
   public:
    using buffer_write_t = std::span<const std::byte>;

    enum class BufferType : Presto::uint8_t { VERTEX, INDEX, UNIFORM };

    [[nodiscard]] Presto::size_t size() const;
    [[nodiscard]] BufferType type() const;

    virtual void write(buffer_write_t bytes, Presto::size_t offset = 0) = 0;

    virtual void bind() = 0;

    virtual ~Buffer() = default;

    Buffer& operator=(Buffer&&) = default;
    Buffer(Buffer&&) = delete;
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    [[nodiscard]] Presto::size_t getWriteSize(Presto::size_t desiredSize,
                                              Presto::size_t startOffset) const;

   protected:
    explicit Buffer(BufferType type, Presto::size_t size);

   private:
    BufferType type_;
    Presto::size_t size_{};
};

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

class Pipeline {
    friend class PipelineBuilder;

   public:
    virtual void bind() = 0;
    virtual void unbind() = 0;

    [[nodiscard]] virtual const PipelineStructure& getStructure() const {
        return pipelineStructure_;
    };

    virtual uniform_index_t getIndex(uniform_name_t name) = 0;

    template <typename T>
    void setUniform(uniform_name_t name, T data) {
        uniform_index_t index{getIndex(name)};
        if (index == PR_INVALID_UNIFORM) {
            PR_ERROR(
                "Unable to set uniform \"{}\" (its index could not be found)",
                name);

            return;
        }
        setUniform(index, data);
    }

    /**
     *  Sets the value of a uniform variable in the pipeline. For OpenGL, these
     * is expected to be used for regular uniform variables.
     */
    virtual void setUniform(uniform_index_t index, Presto::uint32_t value) = 0;

    virtual void setUniform(uniform_index_t index, Presto::int32_t value) = 0;
    virtual void setUniform(uniform_index_t index, Presto::float32_t value) = 0;

    virtual void setUniform(uniform_index_t index, Presto::vec2 value) = 0;
    virtual void setUniform(uniform_index_t index, Presto::vec3 value) = 0;
    virtual void setUniform(uniform_index_t index, Presto::vec4 value) = 0;
    virtual void setUniform(uniform_index_t index, Presto::mat4 value) = 0;

    // [[nodiscard]] bool accepts(const UniformLayout&) const;

    virtual void setUniformBlock(uniform_index_t index,
                                 UniformBuffer& buffer) = 0;

    [[nodiscard]] pipeline_id_t id() const;

    virtual ~Pipeline() = default;

   protected:
    explicit Pipeline(pipeline_id_t id);
    void setId(pipeline_id_t id);

    PipelineStructure pipelineStructure_;
};

}  // namespace Presto
