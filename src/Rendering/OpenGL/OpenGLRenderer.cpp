#include "Rendering/OpenGL/OpenGLRenderer.h"

#include <GL/glew.h>
#include <GL/glext.h>

#include <stdexcept>

#include "GLFWAppWindow.h"
#include "Presto/Log.h"

#include "Rendering/OpenGL/RenderableManager/RenderableManager.h"
#include "utils.h"

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

        /*
                constexpr auto POS_VECTOR_SIZE = 3;
                constexpr auto TRIANGLE_POINT_COUNT = POS_VECTOR_SIZE * 3;

                PointType triangle[] = {0.0f, 0.5f,  0.0f,  0.5f, -0.5f,
                                        0.0f, -0.5f, -0.5f, 0.0f};
                                                                        */
    }

    void OpenGLRenderer::addToRenderPool(draw_info_key renderable) {
        _renderableManager.loadRenderable(renderable);
    }

    void OpenGLRenderer::render(draw_info_key key) {
        auto* ptr = _renderableManager.getRenderable(key);

        if (ptr == nullptr) {
            PR_CORE_ERROR(
                "getRenderable() returned a nullptr for renderable {}. "
                "Skipping drawing.",
                fmt::ptr(key));
            return;
        }

        this->draw(*ptr);
    }

    void OpenGLRenderer::draw(const OpenGlRenderable& renderable) {
        glUseProgram(renderable.shader_program);
        glBindVertexArray(renderable.vao);
        // glDrawArrays(GL_TRIANGLES, renderable.first_index,
        // renderable.vert_count);
        glDrawElements(GL_TRIANGLES, renderable.index_count, GL_UNSIGNED_INT,
                       nullptr);
    }

    void OpenGLRenderer::nextFrame() {
        glfwSwapBuffers(_glfwWindow->getWindowHandle());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    OpenGLRenderer::~OpenGLRenderer() = default;
}  // namespace Presto
