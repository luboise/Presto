#include "Rendering/OpenGL/OpenGLRenderer.h"

#include <GL/glew.h>
#include <GL/glext.h>

#include <stdexcept>

#include "GLFWAppWindow.h"
#include "Presto/Log.h"

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
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle),
                     static_cast<float*>(triangle), GL_STATIC_DRAW);

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
                              POS_VECTOR_SIZE * sizeof(PointType), offset);

        renderable.vertex_array_object = vao;

        GLuint programme = glCreateProgram();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        const char* vertex_shader =
            "#version 400\n"
            "in vec3 vp;"
            "void main() {"
            "  gl_Position = vec4(vp, 1.0);"
            "}";
        glShaderSource(vs, 1, &vertex_shader, nullptr);
        glCompileShader(vs);

        PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(vs),
                       "Shader failed to compile.");
        glAttachShader(programme, vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragment_shader =
            "#version 400\n"
            "out vec4 frag_colour;"
            "void main() {"
            "  frag_colour = vec4(1, 1, 1, 1);"
            "}";
        glShaderSource(fs, 1, &fragment_shader, nullptr);
        glCompileShader(fs);

        PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(fs),
                       "Shader failed to compile.");

        // PR_CORE_ASSERT(true == OpenGLUtils::ShaderCompiledCorrectly(fs),
        //"Unable to compile shader.");
        glAttachShader(programme, fs);

        glLinkProgram(programme);
        renderable.shader_programme = programme;

        renderable.first_index = 0;
        renderable.vert_count = 3;

        _renderable = renderable;
    }

    void OpenGLRenderer::addToRenderPool(draw_info_key key) {}

    void OpenGLRenderer::draw(draw_info_key key) {
        // OpenGlRenderable& renderable = nullptr;
    }

    void OpenGLRenderer::nextFrame() {
        glUseProgram(_renderable.shader_programme);
        glBindVertexArray(_renderable.vertex_array_object);
        glDrawArrays(GL_TRIANGLES, _renderable.first_index,
                     _renderable.vert_count);

        glfwSwapBuffers(_glfwWindow->getWindowHandle());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    OpenGLRenderer::~OpenGLRenderer() = default;
}  // namespace Presto
