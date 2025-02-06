#include "OpenGLDrawManager.h"

#include "Presto/Core.h"
#include "Presto/Core/Constants.h"
#include "Presto/Rendering/RenderTypes.h"

#include "Modules/ModelLoader.h"

#include "DefaultShaders.h"

namespace Presto {
MeshContext* OpenGLDrawManager::getMeshContext(renderer_mesh_id_t id) {
    auto mesh{bufferMap_.find(id)};

    return (mesh == bufferMap_.end()) ? nullptr : &(mesh->second);
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

void OpenGLDrawManager::destroyMeshContext(renderer_mesh_id_t id) {
    auto erased{bufferMap_.erase(id)};
    if (erased == 0) {
        PR_CORE_WARN(
            "A delete was requested for a non-existant mesh with ID {} in "
            "the OpenGL Draw Manager.",
            id);
    }
};

OpenGLTexture* OpenGLDrawManager::getTexture(renderer_texture_id_t id) {
    if (id == UNREGISTERED_RENDER_DATA_ID) {
        PR_WARN(
            "An unregistered texture has been requested in the draw "
            "manager. Using the default texture.");

        return &textureMap_[PR_DEFAULT_TEXTURE];
    }

    auto texture{textureMap_.find(id)};

    return texture == textureMap_.end() ? nullptr : &(texture->second);
};

renderer_mesh_id_t OpenGLDrawManager::createMeshContext(
    const ImportedMesh& mesh) {
    auto buffer{std::make_unique<OpenGLBuffer>(mesh.attributes,
                                               mesh.vertex_count, mesh.bytes,
                                               mesh.indices, mesh.draw_mode)};
    MeshContext context(std::move(buffer));

    auto new_key{++currentKey_};
    auto insertion{bufferMap_.emplace(new_key, buffer)};

    PR_CORE_ASSERT(insertion.second,
                   "Presto failed to insert RenderGroup into the render list.");

    PR_CORE_TRACE("Added new RenderGroup to the render list.");

    return new_key;
};

renderer_texture_id_t OpenGLDrawManager::addTexture(
    const Presto::Image& image) {
    OpenGLTexture tex{image};

    auto new_key{++currentKey_};
    textureMap_.emplace(new_key, std::move(tex));

    return new_key;
};

OpenGLPipeline* OpenGLDrawManager::getPipeline(renderer_pipeline_id_t id) {
    auto pipeline{pipelineMap_.find(id)};

    return (pipeline == pipelineMap_.end()) ? nullptr : &(pipeline->second);
}

/*
    void OpenGLDrawManager::removeMaterial(renderer_material_id_t id) {
        auto erased{materialMap_.erase(id)};
        if (erased == 0) {
            PR_CORE_WARN(
                "A delete was requested for a non-existant material with ID
   {} " "in " "the OpenGL Draw Manager.", id);
        }
    };
        */

void OpenGLDrawManager::setPipeline(renderer_pipeline_id_t id,
                                    OpenGLPipeline&& material) {
    pipelineMap_.emplace(id, std::move(material));
};

void OpenGLDrawManager::setTexture(renderer_texture_id_t id,
                                   OpenGLTexture&& texture) {
    textureMap_.emplace(id, std::move(texture));
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
