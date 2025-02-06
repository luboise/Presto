#include "OpenGLShader.h"
#include "Presto/Utils/File.h"
#include "Rendering/OpenGL/utils.h"

#include "OpenGLDrawManager/DefaultShaders.h"
#include "glm/gtc/type_ptr.hpp"

namespace Presto {
OpenGLShader::OpenGLShader() : shaderProgram_(glCreateProgram()) {};

void OpenGLShader::use() const { glUseProgram(shaderProgram_); };

void OpenGLShader::linkShaderProgram() {
    glLinkProgram(shaderProgram_);
    PR_CORE_ASSERT(OpenGLUtils::ShaderProgramLinkedCorrectly(shaderProgram_),
                   "Shader program failed to link.");

    // Delete shaders after attaching them
    // TODO: Make the base shaders reusable
    glDeleteShader(vertexShader_);
    vertexShader_ = INVALID_SHADER_ID;

    glDeleteShader(fragmentShader_);
    fragmentShader_ = INVALID_SHADER_ID;

    std::vector<shader_property_key_t> property_keys{"view", "projection",
                                                     "transform", "sampler1"};

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

OpenGLShader& OpenGLShader::setShader(const AssetPath& shaderPath,
                                      ShaderStage type) {
    auto data = Utils::File::ReadFile(shaderPath);
    return setShader(data.data(), type);
};

OpenGLShader& OpenGLShader::setShader(const char* data, ShaderStage type) {
    switch (type) {
        case ShaderStage::VERTEX: {
            GLuint vs{glCreateShader(GL_VERTEX_SHADER)};

            glShaderSource(vs, 1, &data, nullptr);
            glCompileShader(vs);

            PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(vs),
                           "Vertex shader failed to compile.");

            glAttachShader(shaderProgram_, vs);

            vertexShader_ = vs;
            break;
        }
        case ShaderStage::FRAGMENT: {
            GLuint fs{glCreateShader(GL_FRAGMENT_SHADER)};

            glShaderSource(fs, 1, &data, nullptr);
            glCompileShader(fs);

            PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(fs),
                           "Fragment shader failed to compile.");

            glAttachShader(shaderProgram_, fs);

            fragmentShader_ = fs;
            break;
        }
        default: {
            PR_CORE_ERROR("Invalid shader set in OpenGL Pipeline.");
        }
    }

    return *this;
}

void OpenGLShader::setUniform(uniform_name_t property, Presto::mat4 value) {
    setMat4(property, glm::value_ptr(value));
};

void OpenGLShader::setUniform(uniform_name_t property, float value) {};

void OpenGLShader::setUniform(uniform_name_t property, int value) {};

void OpenGLShader::setTexture(uniform_name_t property, Texture value) {};

void OpenGLShader::setMat4(uniform_name_t property, float* value) {
    glUniformMatrix4fv(propertyMap_[property], 1, GL_FALSE, value);
};

void OpenGLShader::setUniform(uniform_name_t property, std::uint8_t value) {
    glUniform1i(propertyMap_[property], value);
};

void OpenGLShader::setGlobalUniforms(GlobalUniforms uniforms) {
    setUniform("view", uniforms.view);
    setUniform("projection", uniforms.projection);
};

void OpenGLShader::setObjectUniforms(ObjectUniforms uniforms) {
    setUniform("transform", uniforms.transform);
};
}  // namespace Presto
