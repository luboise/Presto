#include "RenderableManager.h"
#include <stdexcept>

#include "Presto/Core.h"
#include "Presto/Rendering/Vertex.h"
#include "Rendering/OpenGL/utils.h"

namespace Presto {
    OpenGlRenderable* RenderableManager::getRenderable(draw_info_key key) {
        auto x = _renderableMap.find(key);
        if (x == _renderableMap.end()) {
            PR_CORE_ERROR(
                "Unable to find renderable {} in renderable map. Make sure it "
                "is loaded before rendering it. Returning nullptr.",
                fmt::ptr(key));

            return nullptr;
        }

        return &(x->second);
    };

    PR_RESULT RenderableManager::loadRenderable(draw_info_key key) {
        constexpr auto VEC_3 = 3;

        if (_renderableMap.contains(key)) {
            PR_CORE_ERROR(
                "Entity has already been added to render pool. Can't add it "
                "again.");
            return PR_FAILURE;
        }

        // ?? Unsure why this is here
        // Pipeline& pipeline = _graphicsPipelines[0];

        const auto& mesh = key->getMesh();
        const VertexList& vertices = mesh.getVertices();
        const IndexList& indices = mesh.getIndices();

        OpenGlRenderable r{};

        r.vert_count = vertices.size();
        r.index_count = indices.size();

        r.first_index = 0;

        // Vertex buffers
        glGenBuffers(1, &r.vertex_buf);

        // Bind the buffer and then write into it
        glBindBuffer(GL_ARRAY_BUFFER, r.vertex_buf);
        glBufferData(GL_ARRAY_BUFFER, r.vert_count * sizeof(Vertex),
                     vertices.data(), GL_STATIC_DRAW);

        // Bind the indices buffer (EBO)
        glGenBuffers(1, &r.index_buf);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.index_buf);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, r.index_count * sizeof(Index),
                     indices.data(), GL_STATIC_DRAW);

        // Vertex arrays
        glGenVertexArrays(1, &r.vao);
        glBindVertexArray(r.vao);

        // Bind the buffers to the vao
        glBindBuffer(GL_ARRAY_BUFFER, r.vertex_buf);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.index_buf);

        // Set up attribute 0 (pos) from vbo
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, VEC_3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)nullptr);

        // Set up attribute 1 (colour) from vbo
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, VEC_3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)3);

        r.shader_program = glCreateProgram();

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
        glAttachShader(r.shader_program, vs);

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
        glAttachShader(r.shader_program, fs);

        glLinkProgram(r.shader_program);

        // Delete shaders after attaching them
        // TODO: Make the base shaders reusable
        glDeleteShader(vs);
        glDeleteShader(fs);

        auto insertion = _renderableMap.try_emplace(key, r);

        PR_CORE_ASSERT(
            insertion.second,
            "Presto failed to insert renderable {} to the render list.",
            fmt::ptr(key));

        PR_CORE_TRACE("Added {} to the render list.", fmt::ptr(key));

        return PR_RESULT::PR_SUCCESS;
    };

}  // namespace Presto
