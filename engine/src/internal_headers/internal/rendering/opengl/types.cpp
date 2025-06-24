#include "internal/rendering/opengl.h"

#include "./types.h"
#include "presto/core.h"

#include <utility>

constexpr GLenum Pr::OpenGLTypeFromPrestoType(Pr::ShaderDataType type) {
    using namespace Pr;
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

namespace Pr {
constexpr auto INFO_LOG_LENGTH = 512;

bool OpenGLUtils::ShaderCompiledCorrectly(GLuint shader) {
    GLint success = 0;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == 0) {
        std::array<char, INFO_LOG_LENGTH> infoLog{};
        glGetShaderInfoLog(shader, INFO_LOG_LENGTH, nullptr, infoLog.data());

        PR_CORE_ERROR("Shader failed to compile: {}", infoLog.data());
    }

    return success == GL_TRUE;
}

bool OpenGLUtils::ShaderProgramLinkedCorrectly(GLuint shaderProgram) {
    GLint success{};
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLint logLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);

        Pr::string message("", logLength);
        glGetProgramInfoLog(shaderProgram, logLength, &logLength,
                            message.data());

        PR_CORE_ERROR("Shader program failed to link: {}", message);
    }

    return success != 0;
};
GLenum OpenGLUtils::PrestoDrawModeToOpenGLDrawMode(MeshDrawMode mode) {
    switch (mode) {
        case MeshDrawMode::POINTS:
            return GL_POINTS;
        case MeshDrawMode::LINES:
            return GL_LINES;
        case MeshDrawMode::LINE_STRIP:
            return GL_LINE_STRIP;
        case MeshDrawMode::TRIANGLES:
            return GL_TRIANGLES;
        case MeshDrawMode::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
    }
}

ShaderDataType Introspection::getShaderDataType(GLint type, GLint count) {
    switch (type) {
        case GL_INT:
            return ShaderDataType::INT;
        case GL_UNSIGNED_INT:
            return ShaderDataType::UINT;
        case GL_FLOAT_VEC2:
            return ShaderDataType::VEC2;
        case GL_FLOAT_VEC3:
            return ShaderDataType::VEC3;
        case GL_FLOAT_VEC4:
            return ShaderDataType::VEC4;
        case GL_FLOAT_MAT3:
            return ShaderDataType::MAT3;
        case GL_FLOAT_MAT4:
            return ShaderDataType::MAT4;
        case GL_FLOAT:
            switch (count) {
                case 2:
                    return ShaderDataType::VEC2;
                case 3:
                    return ShaderDataType::VEC3;
                case 4:
                    return ShaderDataType::VEC4;
                case 16:
                    return ShaderDataType::MAT4;
                default:
                    return ShaderDataType::FLOAT;
            }
        default:
            PR_ERROR(
                "Unknown shader data type encountered:   type {}   count {}. "
                "Assuming float.",
                type, count);
            return ShaderDataType::FLOAT;
    }
}

UniformVariableType Introspection::getUniformVariableType(GLint type,
                                                          GLint count) {
    switch (type) {
        case GL_SAMPLER_2D:
            return UniformVariableType::TEXTURE;
        case GL_INT:
            return UniformVariableType::INT;
        case GL_UNSIGNED_INT:
            return UniformVariableType::UINT;
        case GL_FLOAT_VEC2:
            return UniformVariableType::VEC2;
        case GL_FLOAT_VEC3:
            return UniformVariableType::VEC3;
        case GL_FLOAT_VEC4:
            return UniformVariableType::VEC4;
        case GL_FLOAT_MAT3:
            return UniformVariableType::MAT3;
        case GL_FLOAT_MAT4:
            return UniformVariableType::MAT4;
        case GL_FLOAT:
            switch (count) {
                case 2:
                    return UniformVariableType::VEC2;
                case 3:
                    return UniformVariableType::VEC3;
                case 4:
                    return UniformVariableType::VEC4;
                case 16:
                    return UniformVariableType::MAT4;
                default:
                    return UniformVariableType::FLOAT;
            }
        default:
            PR_ERROR(
                "Unknown uniform variable type encountered:   type {}   count "
                "{}. "
                "Assuming float.",
                type, count);

            return UniformVariableType::FLOAT;
    }
}

std::vector<PipelineAttribute> Introspection::getAttributesFromShader(
    GLuint program) {
    GLint attrib_count = 0;

    glGetProgramInterfaceiv(program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES,
                            &attrib_count);

    std::vector<PipelineAttribute> attributes(attrib_count);

    std::vector<GLenum> properties{GL_LOCATION, GL_TYPE, GL_ARRAY_SIZE,
                                   GL_NAME_LENGTH};

    std::array<GLint, 4> values{0};

    std::vector<GLchar> name(256);

    Pr::size_t running_offset{0};

    for (GLint index = 0; index < attrib_count; ++index) {
        glGetProgramResourceiv(
            program, GL_PROGRAM_INPUT, index,
            static_cast<GLsizei>(properties.size()), properties.data(),
            static_cast<GLsizei>(properties.size()), nullptr, values.data());

        name.resize(values[3]);
        glGetProgramResourceName(program, GL_PROGRAM_INPUT, index,
                                 static_cast<GLint>(name.size()), nullptr,
                                 name.data());

        PipelineAttribute attribute{
            .layout = static_cast<PR_NUMERIC_ID>(values[0]),
            .type = getShaderDataType(values[1], values[2]),
            .name = Pr::string(name.data()),
            .offset = running_offset};
        running_offset += attribute.size();

        attributes[index] = std::move(attribute);
    }

    return attributes;
};

const auto default_uniform_filter = [](auto index) { return index == -1; };

std::vector<PipelineUniform> Introspection::getUniformsFromShader(
    GLuint program,
    const std::function<bool(GLint)>& filter = default_uniform_filter) {
    GLint uniform_count = 0;

    glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES,
                            &uniform_count);

    std::vector<PipelineUniform> uniforms(uniform_count);

    std::vector<GLenum> properties{GL_LOCATION, GL_TYPE, GL_ARRAY_SIZE,
                                   GL_NAME_LENGTH, GL_BLOCK_INDEX};

    std::array<GLint, 5> values{0};

    std::vector<GLchar> name(256);

    Pr::size_t running_offset{0};

    Pr::size_t real_count{0};

    for (GLint index = 0; index < uniform_count; ++index) {
        glGetProgramResourceiv(
            program, GL_UNIFORM, index, static_cast<GLsizei>(properties.size()),
            properties.data(), static_cast<GLsizei>(properties.size()), nullptr,
            values.data());

        if (!filter(values[4])) {
            continue;
        }

        name.resize(values[3]);
        glGetProgramResourceName(program, GL_UNIFORM, index,
                                 static_cast<GLint>(name.size()), nullptr,
                                 name.data());

        PipelineUniform uniform{
            .location = static_cast<PR_NUMERIC_ID>(values[0]),
            .data_type = getUniformVariableType(values[1], values[2]),
            .name = Pr::string(name.data()),
            .offset = running_offset};

        running_offset += uniform.size();

        uniforms[real_count++] = std::move(uniform);
    }

    uniforms.resize(real_count);

    return uniforms;
};

std::vector<PipelineUniformBlock> Introspection::getUniformBlocksFromShader(
    GLuint program) {
    // Get number of blocks
    GLint block_count = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &block_count);

    std::vector<PipelineUniformBlock> blocks(block_count);

    // List of properties to get about each block
    std::vector<GLenum> block_props{GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE,
                                    GL_NAME_LENGTH, GL_NUM_ACTIVE_VARIABLES};

    std::vector<GLchar> block_name(256);

    // For each uniform block
    for (GLint block_index = 0; block_index < block_count; ++block_index) {
        PipelineUniformBlock uniform_block;

        GLint num_uniforms_in_block{};
        GLint name_length{};

        GLsizei chars_written{};

        glGetActiveUniformBlockiv(program, block_index,
                                  GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                                  &num_uniforms_in_block);

        // Get the name of the block
        glGetActiveUniformBlockiv(program, block_index,
                                  GL_UNIFORM_BLOCK_NAME_LENGTH, &name_length);
        glGetActiveUniformBlockName(program, block_index, name_length,
                                    &chars_written, block_name.data());

        GLint binding{};
        std::vector<GLint> uniform_indices(num_uniforms_in_block);

        // Get the indices of all of the uniforms
        glGetActiveUniformBlockiv(program, block_index,
                                  GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                                  uniform_indices.data());

        for (GLint uniform_index{0}; uniform_index < num_uniforms_in_block;
             ++uniform_index) {
            glGetActiveUniformBlockiv(program, block_index,
                                      GL_UNIFORM_BLOCK_BINDING, &binding);
        }

        std::vector<GLuint> unsigned_indices{uniform_indices.begin(),
                                             uniform_indices.end()};

        auto uniforms{
            getUniformsFromShader(program, [&block_index](auto index) -> bool {
                return index == block_index;
            })};

        uniform_block.name = Pr::string(block_name.data(), chars_written);
        uniform_block.binding = binding;
        uniform_block.uniforms = std::move(uniforms);

        blocks[block_index] = std::move(uniform_block);
    }

    return blocks;
}

std::vector<PipelineUniform> Introspection::getUniforms(GLuint program,
                                                        GLenum base) {
    GLint uniform_count = 0;

    glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES,
                            &uniform_count);

    std::vector<PipelineUniform> uniforms(uniform_count);

    std::vector<GLenum> properties{GL_LOCATION, GL_TYPE, GL_ARRAY_SIZE,
                                   GL_NAME_LENGTH};

    std::array<GLint, 4> values{0};

    std::vector<GLchar> name(256);

    Pr::size_t running_offset{0};

    for (GLint index = 0; index < uniform_count; ++index) {
        glGetProgramResourceiv(
            program, GL_PROGRAM_INPUT, index,
            static_cast<GLsizei>(properties.size()), properties.data(),
            static_cast<GLsizei>(properties.size()), nullptr, values.data());

        name.resize(values[3]);
        glGetProgramResourceName(program, GL_PROGRAM_INPUT, index,
                                 static_cast<GLint>(name.size()), nullptr,
                                 name.data());

        PipelineUniform uniform{
            .location = static_cast<PR_NUMERIC_ID>(values[0]),
            .data_type = getUniformVariableType(values[1], values[2]),
            .name = Pr::string(name.data()),
            .offset = running_offset};
        running_offset += uniform.size();

        uniforms[index] = std::move(uniform);
    }

    return uniforms;
};

}  // namespace Pr
