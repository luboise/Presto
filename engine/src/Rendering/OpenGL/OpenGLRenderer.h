#pragma once

#include <GL/glew.h>
#include "Memory/Allocator.h"
#include "Presto/Core/Constants.h"
// #include "Rendering/OpenGL/OpenGLDrawManager/OpenGLDrawManager.h"

#include "Rendering/Renderer.h"

#include "OpenGLMeshContext.h"

namespace Presto {
class GLFWAppWindow;
class OpenGLPipeline;
/*
   The renderer.
   */
class OpenGLUniformBuffer;

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
                                   Presto::size_t size) override;

    Allocated<UniformBuffer> createUniformBuffer(Presto::size_t size) override;

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

    void onFrameBufferResized() override;

    void setupDebugLogging();

    static void debugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar* message, const void* userParam);
};

}  // namespace Presto
