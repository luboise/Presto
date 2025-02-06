#pragma once

#include "OpenGLTexture.h"
#include "Presto/Core/Constants.h"

#include "Presto/Rendering/RenderTypes.h"

#include "Rendering/OpenGL/OpenGLBuffer.h"
#include "Rendering/OpenGL/OpenGLPipeline.h"
#include "Rendering/OpenGL/OpenGLVAO.h"

#include <GL/glew.h>
#include <map>

namespace Presto {
struct OpenGLPipelineProperties {
    glm::vec4 colour;
    OpenGLTexture texture;
};

struct OpenGLDrawBatch {};

using draw_key = PR_NUMERIC_ID;

struct ImportedMesh;

struct MeshContext {
    std::unique_ptr<OpenGLBuffer> buffer;
    std::map<renderer_pipeline_id_t, OpenGLVAO> vao_map;
};

class OpenGLDrawManager {
    // Materials
    static constexpr PR_NUMERIC_ID PR_MINIMUM_MATERIAL_KEY = 10;

   public:
    renderer_mesh_id_t createMeshContext(const ImportedMesh&);
    MeshContext* getMeshContext(renderer_mesh_id_t);
    void destroyMeshContext(renderer_mesh_id_t);

    OpenGLPipeline* getPipeline(renderer_pipeline_id_t);

    // TODO: Implement custom shaders/materials
    /*
     renderer_material_id_t addMaterial(const Presto::Image& image);
     void removeMaterial(renderer_material_id_t);
            */

    void setPipeline(renderer_pipeline_id_t, OpenGLPipeline&&);

    void setTexture(renderer_texture_id_t id, OpenGLTexture&& texture);

    OpenGLTexture* getTexture(renderer_texture_id_t);
    renderer_texture_id_t addTexture(const Presto::Image& image);
    void removeTexture(renderer_texture_id_t);

   private:
    std::map<renderer_mesh_id_t, MeshContext> bufferMap_;
    std::map<renderer_pipeline_id_t, OpenGLPipeline> pipelineMap_;
    std::map<renderer_texture_id_t, OpenGLTexture> textureMap_;

    // TODO: Change this to a more robust system later
    draw_key currentKey_ = PR_MINIMUM_MATERIAL_KEY;

    GLuint createShaderProgram(const char*);
};
}  // namespace Presto
