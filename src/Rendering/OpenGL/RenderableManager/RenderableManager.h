#pragma once

#include <GL/glew.h>

#include "Presto/Rendering/Renderer.h"

namespace Presto {
    struct OpenGlRenderable {
        GLuint vertex_buffer;
        GLuint index_buffer;
        GLint first_index;
        GLsizei vert_count;
        GLuint shader_programme;
        GLuint vertex_array_object;
    };

    class RenderableManager {
       public:
        OpenGlRenderable& getRenderable(draw_info_key);

        void addRenderable(draw_info_key);
    };
}  // namespace Presto
