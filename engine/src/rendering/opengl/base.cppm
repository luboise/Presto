module;
#include <GL/glew.h>

export module presto.internal.rendering.opengl:base;

// import presto.internal;

import :types;
import :shader;

import presto.utils;
import presto.utils.allocator;
import presto.internal.glfw;

export namespace Pr {

struct OpenGLMeshContext {
    mesh_context_id_t id{-1U};

    OpenGLVAO vao;

    GLenum draw_mode{GL_TRIANGLES};
    // GLsizei index_count{0};
};

class OpenGLRenderer final : public Renderer {
   public:
    explicit OpenGLRenderer(GLFWAppWindow* window);

    void render(MeshRegistrationData& data) override;

    AllocatedPipelineList createDefaultPipelines() override;
    Allocated<PipelineBuilder> getPipelineBuilder() override;

    /*
[[nodiscard]] std::vector<PipelineStructure> getPipelineStructures()
    const override;
            */

    // Destructor
    ~OpenGLRenderer() override;

    Allocated<TextureFactory> getTextureFactory() override;

    Allocated<Buffer> createBuffer(Buffer::BufferType type,
                                   Pr::size_t size) override;

    Allocated<UniformBuffer> createUniformBuffer(Pr::size_t size) override;

    void nextFrame() override;

    // Creates VAO for the mesh so that it can be rendered using that
    // pipeline
    bool createMeshContext(MeshRegistrationData& registration,
                           const PipelineStructure& structure) override;

    // Deleted functions
    OpenGLRenderer(const OpenGLRenderer&) = delete;
    OpenGLRenderer(OpenGLRenderer&&) = delete;
    OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
    OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;

   private:
    Allocated<OpenGLUniformBuffer> globalUniformBuffer_;
    Allocated<OpenGLUniformBuffer> objectUniformBuffer_;

    Allocator<mesh_context_id_t, OpenGLMeshContext> contexts_;

    void updateUniforms();

    void setupDebugLogging();

    void recalculateViewport() override;

    static void debugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar* message, const void* userParam);
};

class OpenGLPipeline final : public Pipeline {
    friend class OpenGLRenderer;
    friend class OpenGLDrawManager;
    friend class OpenGLPipelineBuilder;

    explicit OpenGLPipeline(
        pipeline_id_t id, GLuint vertexShader, GLuint fragmentShader,
        const std::vector<PipelineAttribute>& attributeOverrides = {});
    ~OpenGLPipeline() override;

   public:
    uniform_index_t getIndex(uniform_name_t name) override;

    void setUniform(uniform_index_t index, Pr::uint32_t value) override;

    void setUniform(uniform_index_t index, Pr::int32_t value) override;
    void setUniform(uniform_index_t index, Pr::float32_t value) override;

    void setUniform(uniform_index_t index, Pr::vec2 value) override;
    void setUniform(uniform_index_t index, Pr::vec3 value) override;
    void setUniform(uniform_index_t index, Pr::vec4 value) override;
    void setUniform(uniform_index_t index, Pr::mat4 value) override;
    // void setProperties(const UniformLayout& inStructure) override;

    void setUniformBlock(uniform_index_t index, UniformBuffer& buffer) override;

   private:
    void bind() override;
    void unbind() override;

    // Map which converts the name of a uniform to its external index
    std::map<uniform_name_t, uniform_index_t> nameToIndex_;

    // Map which converts the name of a uniform to its external index
    std::map<uniform_index_t, GLint> indexToBinding_;

    GLuint shaderProgram_{0};
};

class OpenGLPipelineBuilder final : public PipelineBuilderImpl {
    friend class OpenGLRenderer;

   public:
    OpenGLPipelineBuilder& setAttributesOverride(
        std::ranges::range auto&& attributes) {
        this->attributesOverride_ = {attributes.begin(), attributes.end()};

        return *this;
    }

    OpenGLPipelineBuilder& clearAttributesOverride() {
        attributesOverride_.clear();
        return *this;
    }

    PipelineBuilder& setShader(const char* data, ShaderStage type) override {
        switch (type) {
            case ShaderStage::VERTEX: {
                GLuint vs{glCreateShader(GL_VERTEX_SHADER)};

                glShaderSource(vs, 1, &data, nullptr);
                glCompileShader(vs);

                Pr::CoreAssert(OpenGLUtils::ShaderCompiledCorrectly(vs),
                               "Vertex shader failed to compile.");

                vertexShader_.id = vs;
                break;
            }
            case ShaderStage::FRAGMENT: {
                GLuint fs{glCreateShader(GL_FRAGMENT_SHADER)};

                glShaderSource(fs, 1, &data, nullptr);
                glCompileShader(fs);

                Pr::CoreAssert(OpenGLUtils::ShaderCompiledCorrectly(fs),
                               "Fragment shader failed to compile.");

                fragmentShader_.id = fs;
                break;
            }
            default: {
                Pr::CoreLog(ERROR, "Invalid shader set in OpenGL Pipeline.");
            }
        }

        return *dynamic_cast<PipelineBuilder*>(this);
    };

    Allocated<Pipeline> build() override {
        if (id() == PR_PIPELINE_NONE) {
            Pr::CoreLog(ERROR,
                        "A pipeline can't be build using an id of "
                        "PR_PIPELINE_NONE. Unable to build pipeline.");
            return nullptr;
        }
        if (vertexShader_.id == INVALID_SHADER_ID) {
            Pr::CoreLog(
                ERROR,
                "Vertex shader was unassigned when building the pipeline. "
                "Unable to build pipeline.");
            return nullptr;
        }
        if (fragmentShader_.id == INVALID_SHADER_ID) {
            Pr::CoreLog(ERROR,
                        "Fragment shader was unassigned when building the "
                        "pipeline. Unable to build pipeline.");
            return nullptr;
        }

        Allocated<Pipeline> pipeline{new OpenGLPipeline(
            id(), vertexShader_.id, fragmentShader_.id, attributesOverride_)};

        if (!attributesOverride_.empty()) {
        }

        return pipeline;
    };

   private:
    OpenGLPipelineBuilder() = default;

    // OpenGLRenderer* renderer_{nullptr};

    struct ShaderAllocation {
        GLuint id{0};

        explicit ShaderAllocation(GLuint id) : id(id) {};
        ~ShaderAllocation() { glDeleteShader(id); }

        ShaderAllocation(const ShaderAllocation&) = delete;
        ShaderAllocation& operator=(const ShaderAllocation&) = delete;
        ShaderAllocation(ShaderAllocation&&) = default;
        ShaderAllocation& operator=(ShaderAllocation&&) = default;
    };

    std::vector<PipelineAttribute> attributesOverride_;

    ShaderAllocation vertexShader_{INVALID_SHADER_ID};
    ShaderAllocation fragmentShader_{INVALID_SHADER_ID};
};

}  // namespace Pr
