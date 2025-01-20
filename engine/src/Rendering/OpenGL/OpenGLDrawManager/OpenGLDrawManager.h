#pragma once

#include <GL/glew.h>
#include <map>
#include "Presto/Rendering/RenderGroup.h"

namespace Presto {

    struct OpenGLDrawInfo {
        GLuint vertex_buf;
        GLsizei vert_count;

        GLuint index_buf;
        GLsizei index_count;

        GLint first_index;

        GLuint shader_program;
        GLuint vao;

        int draw_mode;
    };

    struct OpenGLDrawBatch {
        std::vector<OpenGLDrawInfo> draws;
    };

    // using draw_key = OpenGLDrawInfo*;

    using draw_key = PR_NUMERIC_ID;

    class OpenGLDrawManager {
       public:
        draw_key createDrawInfo(RenderGroup&& group);

        OpenGLDrawBatch* getDrawBatch(draw_key);

       private:
        std::map<draw_key, OpenGLDrawBatch> drawBatchMap_;

        // TODO: Change this to a more robust system later
        draw_key currentDrawKey_ = 0;

        GLuint createShaderProgram(const char*);
    };
}  // namespace Presto
