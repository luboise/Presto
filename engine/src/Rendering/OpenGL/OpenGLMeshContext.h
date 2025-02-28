#include "Presto/Core/Constants.h"
#include "Rendering/OpenGL/OpenGLVAO.h"

#include <GL/glew.h>

namespace Presto {

struct OpenGLMeshContext {
    mesh_context_id_t id{-1U};

    OpenGLVAO vao;

    GLenum draw_mode{GL_TRIANGLES};
    GLsizei index_count{0};
};

}  // namespace Presto
