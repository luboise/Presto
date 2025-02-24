#include "OpenGLShader.h"
#include "Presto/Utils/File.h"
#include "Rendering/OpenGL/utils.h"

#include "DefaultShaders.h"
#include "glm/gtc/type_ptr.hpp"

namespace Presto {
OpenGLShader::OpenGLShader() : shaderProgram_(glCreateProgram()) {};

void OpenGLShader::use() const { glUseProgram(shaderProgram_); };

void OpenGLShader::linkShaderProgram() {
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

void OpenGLShader::setUniform(uniform_name_t property, Presto::mat4 value) {
    setMat4(property, glm::value_ptr(value));
};

void OpenGLShader::setUniform(uniform_name_t property,
                              Presto::float32_t value) {
    glUniform1f(propertyMap_[property], value);
};

void OpenGLShader::setUniform(uniform_name_t property, Presto::int32_t value) {
    glUniform1i(propertyMap_[property], value);
};

void OpenGLShader::setUniform(uniform_name_t property, Presto::uint32_t value) {
    glUniform1ui(propertyMap_[property], value);
};

void OpenGLShader::setUniform(uniform_name_t property, Presto::vec2 value) {
    glUniform2fv(propertyMap_[property], 1, glm::value_ptr(value));
};

void OpenGLShader::setUniform(uniform_name_t property, Presto::vec3 value) {
    glUniform3fv(propertyMap_[property], 1, glm::value_ptr(value));
};
void OpenGLShader::setUniform(uniform_name_t property, Presto::vec4 value) {
    glUniform4fv(propertyMap_[property], 1, glm::value_ptr(value));
};

void OpenGLShader::setMat4(uniform_name_t property, float* value) {
    glUniformMatrix4fv(propertyMap_[property], 1, GL_FALSE, value);
};

void OpenGLShader::setGlobalUniforms(GlobalUniforms uniforms) {
    setUniform("view", uniforms.view);
    setUniform("projection", uniforms.projection);
};

void OpenGLShader::setObjectUniforms(ObjectUniforms uniforms) {
    setUniform("transform", uniforms.transform);
};
}  // namespace Presto
