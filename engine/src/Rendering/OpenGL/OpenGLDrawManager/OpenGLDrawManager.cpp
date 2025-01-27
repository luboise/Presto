#include "OpenGLDrawManager.h"

#include "Presto/Core.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Rendering/OpenGL/utils.h"

#include "DefaultShaders.h"

namespace Presto {
    OpenGLMeshInfo* OpenGLDrawManager::getMeshInfo(renderer_mesh_id_t key) {
        auto mesh{meshMap_.find(key)};

        return (mesh == meshMap_.end()) ? nullptr : &(mesh->second);
    }

    void OpenGLDrawManager::removeTexture(renderer_texture_id_t id) {
        auto erased{textureMap_.erase(id)};
        if (erased == 0) {
            PR_CORE_WARN(
                "A delete was requested for a non-existant texture with ID {} "
                "in "
                "the OpenGL Draw Manager.",
                id);
        }
    };

    void OpenGLDrawManager::removeMesh(renderer_mesh_id_t id) {
        auto erased{meshMap_.erase(id)};
        if (erased == 0) {
            PR_CORE_WARN(
                "A delete was requested for a non-existant mesh with ID {} in "
                "the OpenGL Draw Manager.",
                id);
        }
    };

    OpenGLTexture* OpenGLDrawManager::getTexture(renderer_texture_id_t id) {
        auto texture{textureMap_.find(id)};

        return texture == textureMap_.end() ? nullptr : &(texture->second);
    };

    renderer_mesh_id_t OpenGLDrawManager::addMesh(const MeshData& mesh) {
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

        OpenGLMeshInfo mesh_info{};

        const VertexList& vertices = mesh.vertices;
        const IndexList& indices = mesh.indices;

        mesh_info.vert_count = vertices.size();
        mesh_info.index_count = indices.size();

        mesh_info.first_index = 0;

        mesh_info.draw_mode = mesh.draw_mode;

        // Create vertex buffer and write into it
        glGenBuffers(1, &mesh_info.vertex_buf);
        glBindBuffer(GL_ARRAY_BUFFER, mesh_info.vertex_buf);
        glBufferData(GL_ARRAY_BUFFER, mesh_info.vert_count * sizeof(Vertex),
                     vertices.data(), GL_STATIC_DRAW);

        // Bind the indices buffer (EBO)
        glGenBuffers(1, &mesh_info.index_buf);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_info.index_buf);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     mesh_info.index_count * sizeof(Index), indices.data(),
                     GL_STATIC_DRAW);

        // Vertex arrays
        glGenVertexArrays(1, &mesh_info.vao);
        glBindVertexArray(mesh_info.vao);

        // Bind the buffers to the vao
        glBindBuffer(GL_ARRAY_BUFFER, mesh_info.vertex_buf);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_info.index_buf);

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

        // mesh_info..texture =

        auto new_key{++currentKey};
        auto insertion = meshMap_.emplace(new_key, mesh_info);

        PR_CORE_ASSERT(
            insertion.second,
            "Presto failed to insert RenderGroup into the render list.");

        PR_CORE_TRACE("Added new RenderGroup to the render list.");

        return new_key;
    };

    renderer_texture_id_t OpenGLDrawManager::addTexture(
        const Presto::Image& image) {
        OpenGLTexture tex{image};

        auto new_key{++currentKey};
        textureMap_.emplace(new_key, std::move(tex));

        return new_key;
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
