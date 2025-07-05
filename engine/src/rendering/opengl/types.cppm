module;
#include <GL/gl.h>

export module presto.internal.rendering.opengl:types;

import presto.core.types;
import presto.core.constants;

import presto.internal.rendering;

export namespace Pr {

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
};

template <ShaderDataType T>
constexpr GLenum OpenGLTypeOf{OpenGLTypeFromPrestoType(T)};

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

struct OpenGLMeshContext {
    mesh_context_id_t id{-1U};

    OpenGLVAO vao;

    GLenum draw_mode{GL_TRIANGLES};
    // GLsizei index_count{0};
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

namespace OpenGLUtils {

GLenum PrestoDrawModeToOpenGLDrawMode(MeshDrawMode mode);

bool ShaderCompiledCorrectly(GLuint shader);
bool ShaderProgramLinkedCorrectly(GLuint shaderProgram);
}  // namespace OpenGLUtils

namespace Introspection {
ShaderDataType getShaderDataType(GLint type, GLint count);
UniformVariableType getUniformVariableType(GLint type, GLint count);
std::vector<PipelineAttribute> getAttributesFromShader(GLuint program);

const auto default_uniform_filter = [](auto index) { return index == -1; };

std::vector<PipelineUniform> getUniformsFromShader(
    GLuint program,
    const std::function<bool(GLint)>& filter = default_uniform_filter);

std::vector<PipelineUniformBlock> getUniformBlocksFromShader(GLuint program);

std::vector<PipelineUniform> getUniforms(GLuint program, GLenum base);
}  // namespace Introspection

}  // namespace Pr
