#pragma once

#include <GL/glew.h>
#include <map>
#include "Presto/Rendering/MeshData.h"

#include "OpenGLTexture.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {
    struct OpenGLMaterialProperties {
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
       public:
        renderer_mesh_id_t addMesh(const MeshData&);
        void removeMesh(renderer_mesh_id_t);

        OpenGLMeshInfo* getMeshInfo(renderer_mesh_id_t);

        renderer_texture_id_t addTexture(const Presto::Image& image);
        void removeTexture(renderer_texture_id_t);

        OpenGLTexture* getTexture(renderer_texture_id_t);

       private:
        std::map<renderer_mesh_id_t, OpenGLMeshInfo> meshMap_;
        std::map<renderer_mesh_id_t, OpenGLTexture> textureMap_;

        // TODO: Change this to a more robust system later
        draw_key currentKey = 0;

        GLuint createShaderProgram(const char*);
    };
}  // namespace Presto
