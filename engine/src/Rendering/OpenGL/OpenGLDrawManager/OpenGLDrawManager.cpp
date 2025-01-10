#include "OpenGLDrawManager.h"

#include "Presto/Core.h"
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
        constexpr auto VEC_2 = 2;

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

        r.draw_mode = data.draw_mode;

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
                              (void*)(offsetof(Vertex, position)));

        // Set up attribute 1 (colour) from vbo
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, VEC_3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, colour));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(1, VEC_3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(1, VEC_2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, tex_coords));

        r.shader_program = glCreateProgram();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);

        constexpr auto DEFAULT_VERTEX_SHADER_PATH = "Shaders/Core/vert.glsl";
        constexpr auto DEFAULT_FRAGMENT_SHADER_PATH = "Shaders/Core/frag.glsl";

        auto vertex_code = Utils::File::ReadFile(DEFAULT_VERTEX_SHADER_PATH);
        PR_ASSERT(vertex_code != "",
                  "Vertex shader at  {}  could not be read, and returned an "
                  "empty file.",
                  DEFAULT_VERTEX_SHADER_PATH);

        const char* sourceCStr = vertex_code.c_str();

        glShaderSource(vs, 1, &sourceCStr, nullptr);
        glCompileShader(vs);

        PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(vs),
                       "Shader failed to compile.");
        glAttachShader(r.shader_program, vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

        auto fragment_code =
            Utils::File::ReadFile(DEFAULT_FRAGMENT_SHADER_PATH);
        PR_ASSERT(fragment_code != "",
                  "Fragment shader at  {}  could not be read, and returned an "
                  "empty file.",
                  DEFAULT_FRAGMENT_SHADER_PATH);

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
        auto insertion = drawInfoMap_.emplace(new_draw_key, r);

        PR_CORE_ASSERT(
            insertion.second,
            "Presto failed to insert RenderData into the render list.");

        PR_CORE_TRACE("Added new RenderData to the render list.");

        return new_draw_key;
    };
}  // namespace Presto