#include "OpenGLRenderer.h"
#include "OpenGLDrawManager/OpenGLDrawManager.h"

#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Rendering/Renderer.h"
#include "Rendering/Utils/RenderingUtils.h"

#include <GL/glew.h>
#include <GL/glext.h>

#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>

#include "Presto/Runtime/GLFWAppWindow.h"

namespace Presto {
    using PointType = float;

    OpenGLRenderer::OpenGLRenderer(GLFWAppWindow* window) {
        this->setWindow(window);
        // glfwMakeContextCurrent(window->getWindowHandle());
        glewExperimental = GL_TRUE;

        auto res = glewInit();
        if (res != GLEW_OK) {
            PR_CORE_ERROR("{}", (const char*)(glewGetErrorString(res)));
            throw std::runtime_error("Unable to initialise GLEW.");
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);  // Smaller = closer

        setupDebugLogging();

        drawManager_ = std::make_unique<OpenGLDrawManager>();

        /*
                constexpr auto POS_VECTOR_SIZE = 3;
                constexpr auto TRIANGLE_POINT_COUNT = POS_VECTOR_SIZE * 3;

                PointType triangle[] = {0.0f, 0.5f,  0.0f,  0.5f, -0.5f,
                                        0.0f, -0.5f, -0.5f, 0.0f};
                                                                        */
    }

    render_data_id_t OpenGLRenderer::registerRenderGroup(
        const RenderGroup& data) {
        RenderGroup copy{data};
        return drawManager_->createDrawInfo(std::move(copy));
    }

    render_data_id_t OpenGLRenderer::registerRenderGroup(RenderGroup&& data) {
        return drawManager_->createDrawInfo(std::move(data));
    }

    void OpenGLRenderer::render(render_data_id_t id, glm::mat4 transform) {
        auto* ptr = drawManager_->getDrawBatch(id);

        // TODO: Implement drawing using the transform
        if (ptr == nullptr) {
            PR_CORE_ERROR(
                "getRenderable() returned a nullptr for renderable {}. "
                "Skipping drawing.",
                id);
            return;
        }

        this->draw(*ptr, transform);
    }

    void OpenGLRenderer::unregisterMesh(render_data_id_t id) {
        // TODO: Implement unregister
    }

    void OpenGLRenderer::draw(const OpenGLDrawBatch& batch,
                              const glm::mat4& transform) {
        for (const auto& draw_data : batch.draws) {
            auto slot = 0;
            draw_data.mat_props.texture.bind(0);

            PR_CORE_ASSERT(
                draw_data.mat_props.texture.isLoaded(),
                "Draw data is used without being loaded in OpenGL Renderer.");
            glUseProgram(draw_data.shader_program);

            // TODO: Make this a one-off function call
            {
                GLint sampler{
                    glGetUniformLocation(draw_data.shader_program, "sampler1")};
                PR_CORE_ASSERT(sampler != -1,
                               "Sampler1 is not found in the shader program.");
                // Set sampler1 to be using texture slot 0
                glUniform1i(sampler, slot);
            }

            ShaderMatrices mats{};

            constexpr glm::float32 FOV_Y = 90;

            mats.view = renderViewMatrix_;
            mats.projection = RenderingUtils::getProjectionMatrix(
                FOV_Y, _glfwWindow->GetWidth(), _glfwWindow->GetHeight());
            mats.transform = transform;

            GLint view = glGetUniformLocation(draw_data.shader_program, "view");
            GLint projection =
                glGetUniformLocation(draw_data.shader_program, "projection");
            GLint transform =
                glGetUniformLocation(draw_data.shader_program, "transform");

            glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(mats.view));
            glUniformMatrix4fv(projection, 1, GL_FALSE,
                               glm::value_ptr(mats.projection));
            glUniformMatrix4fv(transform, 1, GL_FALSE,
                               glm::value_ptr(mats.transform));

            glBindVertexArray(draw_data.vao);
            glDrawElements(draw_data.draw_mode, draw_data.index_count,
                           GL_UNSIGNED_INT, nullptr);
        }
    }

    void OpenGLRenderer::nextFrame() {
        glfwSwapBuffers(_glfwWindow->getWindowHandle());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    OpenGLRenderer::~OpenGLRenderer() = default;

    void GLAPIENTRY OpenGLRenderer::debugCallback(GLenum source, GLenum type,
                                                  GLuint id, GLenum severity,
                                                  GLsizei length,
                                                  const GLchar* message,
                                                  const void* userParam) {
        PR_CORE_TRACE("OpenGL Debug Message (ID: {}): {}", id, message);
    }

    void OpenGLRenderer::setupDebugLogging() {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glEnable(GL_DEBUG_OUTPUT);
        // Ignore messages that are not errors, warnings, or deprecated behavior
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW,
                              0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr,
                              GL_FALSE);

        // Allow error, warning, and deprecated behavior messages
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE,
                              0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PORTABILITY,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);

        glDebugMessageCallback(debugCallback, nullptr);
    };
}  // namespace Presto
