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

        drawManager_ = std::make_unique<OpenGLDrawManager>();

        /*
                constexpr auto POS_VECTOR_SIZE = 3;
                constexpr auto TRIANGLE_POINT_COUNT = POS_VECTOR_SIZE * 3;

                PointType triangle[] = {0.0f, 0.5f,  0.0f,  0.5f, -0.5f,
                                        0.0f, -0.5f, -0.5f, 0.0f};
                                                                        */
    }

    render_data_id_t OpenGLRenderer::registerMesh(const RenderData& data) {
        RenderData copy{data};
        return drawManager_->createDrawInfo(std::move(copy));
    }

    render_data_id_t OpenGLRenderer::registerMesh(RenderData&& data) {
        return drawManager_->createDrawInfo(std::move(data));
    }

    void OpenGLRenderer::render(render_data_id_t id, glm::mat4 transform) {
        auto* ptr = drawManager_->getDrawInfo(id);

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

    void OpenGLRenderer::draw(const OpenGLDrawInfo& renderable,
                              const glm::mat4& transform) {
        GLint view = glGetUniformLocation(renderable.shader_program, "view");
        GLint projection =
            glGetUniformLocation(renderable.shader_program, "projection");

        glUseProgram(renderable.shader_program);

        ShaderMatrices mats{};

        constexpr glm::float32 FOV_Y = 90;

        mats.view = renderViewMatrix_ * transform;

        mats.projection = RenderingUtils::getProjectionMatrix(
            FOV_Y, _glfwWindow->GetWidth(), _glfwWindow->GetHeight());

        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(mats.view));
        glUniformMatrix4fv(projection, 1, GL_FALSE,
                           glm::value_ptr(mats.projection));

        // Commence drawing
        glBindVertexArray(renderable.vao);
        glDrawElements(GL_TRIANGLES, renderable.index_count, GL_UNSIGNED_INT,
                       nullptr);
    }

    void OpenGLRenderer::nextFrame() {
        glfwSwapBuffers(_glfwWindow->getWindowHandle());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    OpenGLRenderer::~OpenGLRenderer() = default;
}  // namespace Presto
