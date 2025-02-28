#pragma once

#include <GL/glew.h>

#include "Presto/Rendering/AttributeTypes.h"
#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/UniformTypes.h"

namespace Presto {
namespace Introspection {

ShaderDataType getShaderDataType(GLint type, GLint count) {
    switch (type) {
        case GL_INT:
            return ShaderDataType::INT;
        case GL_UNSIGNED_INT:
            return ShaderDataType::UINT;
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
            // TODO: Add proper error handling here
            return ShaderDataType::FLOAT;
    }
}

UniformVariableType getUniformVariableType(GLint type, GLint count) {
    switch (type) {
        case GL_INT:
            return UniformVariableType::INT;
        case GL_UNSIGNED_INT:
            return UniformVariableType::UINT;
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
            // TODO: Add proper error handling here
            return UniformVariableType::FLOAT;
    }
}

std::vector<PipelineAttribute> getAttributesFromShader(GLuint program) {
    GLint attrib_count = 0;

    glGetProgramInterfaceiv(program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES,
                            &attrib_count);

    std::vector<PipelineAttribute> attributes(attrib_count);

    std::vector<GLenum> properties{GL_LOCATION, GL_TYPE, GL_ARRAY_SIZE,
                                   GL_NAME_LENGTH};

    std::array<GLint, 4> values{0};

    std::vector<GLchar> name(256);

    Presto::size_t running_offset{0};

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
            .name = std::string(name.data()),
            .offset = running_offset};
        running_offset += attribute.size();

        attributes[index] = std::move(attribute);
    }

    return attributes;
};

std::vector<PipelineUniform> getUniforms(GLuint program, GLenum base) {
    GLint uniform_count = 0;

    glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES,
                            &uniform_count);

    std::vector<PipelineUniform> uniforms(uniform_count);

    std::vector<GLenum> properties{GL_LOCATION, GL_TYPE, GL_ARRAY_SIZE,
                                   GL_NAME_LENGTH};

    std::array<GLint, 4> values{0};

    std::vector<GLchar> name(256);

    Presto::size_t running_offset{0};

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
            .name = std::string(name.data()),
            .offset = running_offset};
        running_offset += uniform.size();

        uniforms[index] = std::move(uniform);
    }

    return uniforms;
};

std::vector<PipelineUniform> getUniformsFromShader(GLuint program) {
    GLint uniform_count = 0;

    glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES,
                            &uniform_count);

    std::vector<PipelineUniform> uniforms(uniform_count);

    std::vector<GLenum> properties{GL_LOCATION, GL_TYPE, GL_ARRAY_SIZE,
                                   GL_NAME_LENGTH};

    std::array<GLint, 4> values{0};

    std::vector<GLchar> name(256);

    Presto::size_t running_offset{0};

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
            .name = std::string(name.data()),
            .offset = running_offset};
        running_offset += uniform.size();

        uniforms[index] = std::move(uniform);
    }

    return uniforms;
};

std::vector<PipelineUniformBlock> getUniformBlocksFromShader(GLuint program) {
    return {};

    /*
// Get number of blocks
GLint block_count = 0;
glGetProgramInterfaceiv(program, GL_UNIFORM_BLOCK, GL_ACTIVE_UNIFORM_BLOCKS,
                        &block_count);

std::vector<PipelineUniformBlock> blocks(block_count);

// List of properties to get about each block
std::vector<GLenum> properties{GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE,
                               GL_NAME_LENGTH, GL_NUM_ACTIVE_VARIABLES};

std::array<GLint, 4> values{0};

std::vector<GLchar> name(256);

for (GLint index = 0; index < block_count; ++index) {
    // Get properties
    glGetProgramResourceiv(
        program, GL_UNIFORM_BLOCK, index,
        static_cast<GLsizei>(properties.size()), properties.data(),
        static_cast<GLsizei>(properties.size()), nullptr, values.data());

    // Get name
    name.resize(values[2]);
    glGetProgramResourceName(program, GL_UNIFORM_BLOCK, index,
                             static_cast<GLint>(name.size()), nullptr,
                             name.data());

    std::vector<PipelineUniform> uniforms;
    for (auto uniform_index = 0; uniform_index < values[3];
         ++uniform_index) {
        GLint uniform_count{};
        glGetActiveUniformBlockiv(program, uniform_index,
                                  GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                                  &uniform_count);

        std::vector<GLint> uniform_indices(uniform_count);

        glGetActiveUniformBlockiv(program, blockIndex,
                                  GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                                  uniform_indices.data());

        glGetActiveUniformsiv(program);
    }

    PipelineUniformBlock uniform{
        .location = static_cast<PR_NUMERIC_ID>(values[0]),
        .type = getShaderDataType(values[1], values[2]),
        .name = std::string(name.data()),
    };

    blocks[index] = std::move(attribute);
}

    return blocks;
    */
}

}  // namespace Introspection

}  // namespace Presto
