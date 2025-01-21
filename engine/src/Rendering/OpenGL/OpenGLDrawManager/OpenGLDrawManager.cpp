#include "OpenGLDrawManager.h"

#include "Presto/Core.h"
#include "Rendering/OpenGL/utils.h"

#include "DefaultShaders.h"

namespace Presto {
    OpenGLDrawBatch* OpenGLDrawManager::getDrawBatch(draw_key key) {
        auto x = drawBatchMap_.find(key);
        if (x == drawBatchMap_.end()) {
            PR_CORE_ERROR(
                "Unable to find renderable {} in renderable map. Make sure it "
                "is loaded before rendering it. Returning nullptr.",
                key);

            return nullptr;
        }

        return &(x->second);
    }

    draw_key OpenGLDrawManager::createDrawInfo(RenderGroup&& group) {
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

        OpenGLDrawBatch batch{};

        for (const auto& data : group.render_list) {
            OpenGLDrawInfo draw_info{};

            const VertexList& vertices = data.vertices;
            const IndexList& indices = data.indices;

            draw_info.vert_count = vertices.size();
            draw_info.index_count = indices.size();

            draw_info.first_index = 0;

            draw_info.draw_mode = data.draw_mode;

            // Create vertex buffer and write into it
            glGenBuffers(1, &draw_info.vertex_buf);
            glBindBuffer(GL_ARRAY_BUFFER, draw_info.vertex_buf);
            glBufferData(GL_ARRAY_BUFFER, draw_info.vert_count * sizeof(Vertex),
                         vertices.data(), GL_STATIC_DRAW);

            // Bind the indices buffer (EBO)
            glGenBuffers(1, &draw_info.index_buf);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, draw_info.index_buf);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         draw_info.index_count * sizeof(Index), indices.data(),
                         GL_STATIC_DRAW);

            // Vertex arrays
            glGenVertexArrays(1, &draw_info.vao);
            glBindVertexArray(draw_info.vao);

            // Bind the buffers to the vao
            glBindBuffer(GL_ARRAY_BUFFER, draw_info.vertex_buf);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, draw_info.index_buf);

            constexpr auto STRIDE = sizeof(Vertex);

            // Set up attribute 0 (pos) from vbo
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, VEC_3, GL_FLOAT, GL_FALSE, STRIDE,
                                  (void*)(offsetof(Vertex, position)));

            // Set up attribute 1 (colour) from vbo
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, VEC_3, GL_FLOAT, GL_FALSE, STRIDE,
                                  (void*)offsetof(Vertex, colour));

            // Set up attribute 2 (normal) from vbo
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, VEC_3, GL_FLOAT, GL_FALSE, STRIDE,
                                  (void*)offsetof(Vertex, normal));

            // Set up attribute 3 (texture coordinates) from vbo
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, VEC_2, GL_FLOAT, GL_FALSE, STRIDE,
                                  (void*)offsetof(Vertex, tex_coords));

            draw_info.shader_program = glCreateProgram();

            GLuint vs = glCreateShader(GL_VERTEX_SHADER);

            const char* sourceCStr = DEFAULT_VERTEX_SHADER;

            glShaderSource(vs, 1, &sourceCStr, nullptr);
            glCompileShader(vs);

            PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(vs),
                           "Shader failed to compile.");
            glAttachShader(draw_info.shader_program, vs);

            GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

            const char* sourceCStr2 = DEFAULT_FRAGMENT_SHADER;

            glShaderSource(fs, 1, &sourceCStr2, nullptr);
            glCompileShader(fs);

            PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(fs),
                           "Shader failed to compile.");

            glAttachShader(draw_info.shader_program, fs);

            glLinkProgram(draw_info.shader_program);
            PR_CORE_ASSERT(OpenGLUtils::ShaderProgramLinkedCorrectly(
                               draw_info.shader_program),
                           "Shader program failed to link.");

            // Delete shaders after attaching them
            // TODO: Make the base shaders reusable
            glDeleteShader(vs);
            glDeleteShader(fs);

            draw_info.mat_props.texture = OpenGLTexture(data.material->image);

            batch.draws.push_back(std::move(draw_info));
        }

        auto new_draw_key = ++currentDrawKey_;
        auto insertion = drawBatchMap_.emplace(new_draw_key, std::move(batch));

        PR_CORE_ASSERT(
            insertion.second,
            "Presto failed to insert RenderGroup into the render list.");

        PR_CORE_TRACE("Added new RenderGroup to the render list.");

        return new_draw_key;
    };
}  // namespace Presto

/*
const std::string DEFAULT_VERTEX_SHADER_PATH =
Utils::File::getFullPath("Shaders/Core/vert.glsl");

const std::string DEFAULT_FRAGMENT_SHADER_PATH =
Utils::File::getFullPath("Shaders/Core/frag.glsl");

auto vertex_code =
Utils::File::ReadFile(DEFAULT_VERTEX_SHADER_PATH);

PR_ASSERT(
    vertex_code != "",
    "Vertex shader at  {}  could not be read, and returned an "
    "empty file.",
    DEFAULT_VERTEX_SHADER_PATH);
        */

/*
auto fragment_code =
Utils::File::ReadFile(DEFAULT_FRAGMENT_SHADER_PATH);
PR_ASSERT(
fragment_code != "",
"Fragment shader at  {}  could not be read, and returned an "
"empty file.",
DEFAULT_FRAGMENT_SHADER_PATH);
        */
