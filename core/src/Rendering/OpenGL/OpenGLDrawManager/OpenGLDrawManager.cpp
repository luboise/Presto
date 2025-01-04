#include "OpenGLDrawManager.h"

#include "PrestoCore/Modules/ResourceManager.h"

#include "PrestoCore/Core.h"
#include "Rendering/OpenGL/utils.h"

namespace Presto {
    OpenGLDrawInfo* OpenGLDrawManager::getDrawInfo(draw_key key) {
        auto x = drawInfoMap_.find(key);
        if (x == drawInfoMap_.end()) {
            PR_CORE_ERROR(
                "Unable to find renderable {} in renderable map. Make sure it "
                "is loaded before rendering it. Returning nullptr.",
                key);

            return nullptr;
        }

        return &(x->second);
    }

    draw_key OpenGLDrawManager::createDrawInfo(RenderData&& data) {
        constexpr auto VEC_3 = 3;

        // TODO: Potentially reimplement this later if it has a measurable
        // benefit to performance
        /*
if (renderableMap_.contains(data)) {
    PR_CORE_ERROR(
        "Entity has already been added to render pool. Can't add it "
        "again.");
    return PR_FAILURE;
}
        */

        const VertexList& vertices = data.vertices;
        const IndexList& indices = data.indices;

        OpenGLDrawInfo r{};

        r.vert_count = vertices.size();
        r.index_count = indices.size();

        r.first_index = 0;

        // Create vertex buffer and write into it
        glGenBuffers(1, &r.vertex_buf);
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
                              (void*)(offsetof(Vertex, pos)));

        // Set up attribute 1 (colour) from vbo
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, VEC_3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, colour));
        r.shader_program = glCreateProgram();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);

        auto vertex_code = ResourceManager::ReadFile("Shaders/Core/vert.glsl");

        const char* sourceCStr = vertex_code.c_str();

        glShaderSource(vs, 1, &sourceCStr, nullptr);
        glCompileShader(vs);

        PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(vs),
                       "Shader failed to compile.");
        glAttachShader(r.shader_program, vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

        auto fragment_code =
            ResourceManager::ReadFile("Shaders/Core/frag.glsl");
        const char* sourceCStr2 = fragment_code.c_str();

        glShaderSource(fs, 1, &sourceCStr2, nullptr);
        glCompileShader(fs);

        PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(fs),
                       "Shader failed to compile.");

        // PR_CORE_ASSERT(true ==
        // OpenGLUtils::ShaderCompiledCorrectly(fs),
        //"Unable to compile shader.");
        glAttachShader(r.shader_program, fs);

        glLinkProgram(r.shader_program);

        // Delete shaders after attaching them
        // TODO: Make the base shaders reusable
        glDeleteShader(vs);
        glDeleteShader(fs);

        auto new_draw_key = ++currentDrawKey_;
        auto insertion = drawInfoMap_.try_emplace(new_draw_key, r);

        PR_CORE_ASSERT(
            insertion.second,
            "Presto failed to insert RenderData into the render list.");

        PR_CORE_TRACE("Added new RenderData to the render list.");

        return new_draw_key;
    };

}  // namespace Presto
