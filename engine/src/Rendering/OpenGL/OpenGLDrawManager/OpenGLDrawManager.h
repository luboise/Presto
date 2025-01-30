#pragma once

#include <GL/glew.h>
#include <map>
#include "Presto/Core/Constants.h"

#include "OpenGLTexture.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Rendering/OpenGL/OpenGLPipeline.h"

namespace Presto {
    struct OpenGLPipelineProperties {
        glm::vec4 colour;
        OpenGLTexture texture;
    };

    struct OpenGLMeshInfo {
        GLuint vertex_buf{};
        GLsizei vert_count{};

        GLuint index_buf{};
        GLsizei index_count{};

        GLint first_index{};

        GLuint shader_program{};
        GLuint vao{};

        int draw_mode{};
    };

    struct OpenGLDrawBatch {};

    using draw_key = PR_NUMERIC_ID;

    class OpenGLDrawManager {
        // Materials
        static constexpr PR_NUMERIC_ID PR_MINIMUM_MATERIAL_KEY = 10;

       public:
        OpenGLMeshInfo* getMeshInfo(renderer_mesh_id_t);
        renderer_mesh_id_t addMesh(const MeshData&);
        void removeMesh(renderer_mesh_id_t);

        OpenGLPipeline* getMaterial(renderer_pipeline_id_t);

        // TODO: Implement custom shaders/materials
        /*
         renderer_material_id_t addMaterial(const Presto::Image& image);
         void removeMaterial(renderer_material_id_t);
                */

        void setMaterial(renderer_pipeline_id_t, OpenGLPipeline&&);

        void setTexture(renderer_texture_id_t id, OpenGLTexture&& texture);

        OpenGLTexture* getTexture(renderer_texture_id_t);
        renderer_texture_id_t addTexture(const Presto::Image& image);
        void removeTexture(renderer_texture_id_t);

       private:
        std::map<renderer_mesh_id_t, OpenGLMeshInfo> meshMap_;
        std::map<renderer_pipeline_id_t, OpenGLPipeline> materialMap_;
        std::map<renderer_texture_id_t, OpenGLTexture> textureMap_;

        // TODO: Change this to a more robust system later
        draw_key currentKey_ = PR_MINIMUM_MATERIAL_KEY;

        GLuint createShaderProgram(const char*);
    };
}  // namespace Presto
