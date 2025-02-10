#include "OpenGLPipeline.h"

#include "Presto/Rendering/UniformTypes.h"
#include "Rendering/OpenGL/OpenGLDrawManager/OpenGLTexture.h"
#include "Rendering/OpenGL/OpenGLShader.h"
#include "Rendering/OpenGL/utils.h"
#include "Rendering/Utils/RenderingUtils.h"

namespace Presto {
using Presto::UniformVariableType;

OpenGLPipeline::OpenGLPipeline(ShaderPtr& ptr) {
    PR_ASSERT(ptr->ready(), "Unable to create Material from unready shader.");
    shader_ = ptr;
};

OpenGLPipeline::OpenGLPipeline(GLuint vertexShader_, GLuint fragmentShader_)
    : shaderProgram_(glCreateProgram()) {
    glAttachShader(shaderProgram_, vertexShader_);
    glAttachShader(shaderProgram_, fragmentShader_);

    glLinkProgram(shaderProgram_);
    PR_CORE_ASSERT(OpenGLUtils::ShaderProgramLinkedCorrectly(shaderProgram_),
                   "Shader program failed to link.");

    // Delete shaders after attaching them
    // TODO: Make the base shaders reusable

    this->attributes_ = getAttributesFromShader(shaderProgram_);
    this->uniforms_ = getuniformsFromShader(shaderProgram_);

    for (const auto& key : property_keys) {
        propertyMap_[key] = glGetUniformLocation(shaderProgram_, key.data());
    }

    // TODO: Delete this once its been tested
    /*
view = glGetUniformLocation(shaderProgram_, "view");
view = glGetUniformLocation(shaderProgram_, "projection");
view = glGetUniformLocation(shaderProgram_, "transform");
    */
    setReady(true);
};

void OpenGLPipeline::bind() { glUseProgram(shaderProgram_); }

void OpenGLPipeline::unbind() { glUseProgram(0); }

void OpenGLPipeline::setProperty(std::string /*property*/,
                                 const void* /*value*/) {};

// ShaderPtr& OpenGLPipeline::getShader() { return shader_; };

/*void OpenGLPipeline::setDiffuse(OpenGLTexture* image) {*/
/*    diffuseTexture_ = image;*/
/*};*/

void OpenGLPipeline::setProperties(const MaterialStructure& inStructure) {
    std::vector<std::byte> bytes_to_write();
    for (const auto& uniform : this->uniforms_) {
        const auto* in_property{inStructure.getProperty(name)};
    }

    /*
for (const auto& [name, outProperty] : shader_->getInfo().uniforms) {
    const auto* in_property{inStructure.getProperty(name)};

    if (in_property == nullptr) {
        PR_CORE_WARN(std::format(
            "Missing uniform value \"{}\" required by pipeline {}. "
            "Skipping this property.",
            name, this->id_));
        continue;
    }

#define SWITCH_CASE(T)                                                     \
case T: {                                                              \
    using TrueType = GPUTypeOf<T>;                                     \
    this->shader_->setUniform(name, in_property->data.as<TrueType>()); \
    break;                                                             \
}

    switch (in_property->type) {
        SWITCH_CASE(UniformVariableType::INT)
        SWITCH_CASE(UniformVariableType::UINT)
        SWITCH_CASE(UniformVariableType::FLOAT)

        SWITCH_CASE(UniformVariableType::VEC2)
        SWITCH_CASE(UniformVariableType::VEC3)
        SWITCH_CASE(UniformVariableType::VEC4)

        SWITCH_CASE(UniformVariableType::MAT4)
        SWITCH_CASE(UniformVariableType::IMAGE)
    }
}

#undef SWITCH_CASE
*/
}

OpenGLPipeline::~OpenGLPipeline() { glDeleteProgram(shaderProgram_); };

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

std::vector<PipelineAttribute> OpenGLPipeline::getAttributesFromShader(
    GLuint program) {
    GLint attrib_count = 0;

    glGetProgramInterfaceiv(program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES,
                            &attrib_count);

    std::vector<PipelineAttribute> attributes(attrib_count);

    std::vector<GLenum> properties{GL_LOCATION, GL_TYPE, GL_ARRAY_SIZE,
                                   GL_NAME_LENGTH};

    std::array<GLint, 4> values{0};

    std::vector<GLchar> name(256);

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
        };

        attributes[index] = std::move(attribute);
    }

    return attributes;
};

std::vector<PipelineUniform> OpenGLPipeline::getUniformsFromShader(
    GLuint program) {
    GLint uniform_count = 0;

    glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES,
                            &uniform_count);

    std::vector<PipelineUniform> uniforms(uniform_count);

    std::vector<GLenum> properties{GL_LOCATION, GL_TYPE, GL_ARRAY_SIZE,
                                   GL_NAME_LENGTH};

    std::array<GLint, 4> values{0};

    std::vector<GLchar> name(256);

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
            .binding = static_cast<PR_NUMERIC_ID>(values[0]),
            .type = getShaderDataType(values[1], values[2]),
            .name = std::string(name.data()),
        };

        uniforms[index] = std::move(attribute);
    }

    return uniforms;
};

}  // namespace Presto
