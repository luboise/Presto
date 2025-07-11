module;
#include <GL/glew.h>

export module presto.internal.rendering.opengl;

import presto.internal;

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

}  // namespace Pr
