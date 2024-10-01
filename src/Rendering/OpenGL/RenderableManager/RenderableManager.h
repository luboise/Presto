#pragma once

#include <GL/glew.h>
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {
    struct OpenGlRenderable {
        GLuint vertex_buf;
        GLsizei vert_count;

        GLuint index_buf;
        GLsizei index_count;

        GLint first_index;

        GLuint shader_program;
        GLuint vao;
    };

    class RenderableManager {
       public:
        PR_RESULT loadRenderable(draw_info_key);
        OpenGlRenderable* getRenderable(draw_info_key);

       private:
        DrawInfoMap<OpenGlRenderable> _renderableMap;
        GLuint createShaderProgram(const char*);
    };
}  // namespace Presto
