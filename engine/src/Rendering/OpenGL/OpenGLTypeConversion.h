#pragma once

#include <GL/glew.h>

namespace Presto {

constexpr GLenum OpenGLTypeFromPrestoType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::SHORT:
            return GL_SHORT;
        case ShaderDataType::USHORT:
            return GL_UNSIGNED_SHORT;
        case ShaderDataType::INT:
            return GL_INT;
        case ShaderDataType::UINT:
            return GL_UNSIGNED_INT;

        case ShaderDataType::VEC2:
        case ShaderDataType::VEC3:
        case ShaderDataType::VEC4:
        case ShaderDataType::FLOAT:
        case ShaderDataType::MAT3:
        case ShaderDataType::MAT4:
            return GL_FLOAT;

        case ShaderDataType::DVEC2:
        case ShaderDataType::DVEC3:
        case ShaderDataType::DVEC4:
        case ShaderDataType::DMAT3:
        case ShaderDataType::DMAT4:
        case ShaderDataType::DOUBLE:
            return GL_DOUBLE;

            // Error case
        default:
            return GL_NONE;
    }
}

template <ShaderDataType T>
constexpr GLenum OpenGLTypeOf{OpenGLTypeFromPrestoType(T)};

}  // namespace Presto
