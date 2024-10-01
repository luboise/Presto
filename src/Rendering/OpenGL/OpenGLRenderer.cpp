#include "Rendering/OpenGL/OpenGLRenderer.h"

#include <GL/glew.h>
#include <stdexcept>

#include "GLFWAppWindow.h"
#include "Presto/Log.h"
#include "spdlog/fmt/bundled/core.h"

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
        glDepthFunc(GL_LESS);  // Smaller closer

        OpenGlRenderable renderable{};

        constexpr auto POS_VECTOR_SIZE = 3;
        constexpr auto TRIANGLE_POINT_COUNT = POS_VECTOR_SIZE * 3;

        PointType triangle[] = {0.0f, 0.5f,  0.0f,  0.5f, -0.5f,
                                0.0f, -0.5f, -0.5f, 0.0f};

        // Vertex buffers
        GLuint vbo = 0;
        glGenBuffers(1, &vbo);

        // Bind the buffer and then write into it
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(PointType) * TRIANGLE_POINT_COUNT,
                     static_cast<float*>(triangle), GL_STATIC_READ);

        renderable.vertex_buffer = vbo;

        // Vertex arrays
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Set up attribute 0
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        constexpr void* offset = nullptr;
        glVertexAttribPointer(0, POS_VECTOR_SIZE, GL_FLOAT, GL_FALSE,
                              POS_VECTOR_SIZE, offset);

        _renderable = renderable;
    }

    void OpenGLRenderer::addToRenderPool(draw_info_key key) {}

    void OpenGLRenderer::draw(draw_info_key key) {
        // OpenGlRenderable& renderable = nullptr;
    }

    void OpenGLRenderer::nextFrame() {
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(_glfwWindow->getWindowHandle());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    OpenGLRenderer::~OpenGLRenderer() = default;
}  // namespace Presto
