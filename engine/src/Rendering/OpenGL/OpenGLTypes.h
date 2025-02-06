#pragma once

#include <GL/gl.h>
#include "Presto/Rendering/AttributeTypes.h"
#include "Presto/Rendering/VertexAttribute.h"  // IWYU pragma: export

namespace Presto {
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

constexpr GLint getGLAttribType(ShaderDataSubType subType) {
    switch (subType) {
        case ShaderDataSubType::INT:
            return GL_INT;
        case ShaderDataSubType::UINT:
            return GL_UNSIGNED_INT;
        case ShaderDataSubType::FLOAT:
            return GL_FLOAT;
    }
}

}  // namespace Presto
