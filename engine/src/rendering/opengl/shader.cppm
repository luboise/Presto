module;
#include <GL/glew.h>

export module presto.internal.rendering.opengl:shader;

import glm;

import :types;
import :buffer;

import presto.assets.asset;
import presto.utils;

import std;

namespace Pr {

export class OpenGLShader final : public Shader {
   public:
    using shader_property_key_t = Pr::string;

    using opengl_shader_ptr_t = std::shared_ptr<OpenGLShader>;

    OpenGLShader();

    void use() const override;

    void setGlobalUniforms(GlobalUniforms uniforms) override;
    void setObjectUniforms(ObjectUniforms uniforms) override;

    void setUniform(uniform_name_t property, Pr::uint32_t value) override;

    void setUniform(uniform_name_t property, Pr::int32_t value) override;
    void setUniform(uniform_name_t property, Pr::float32_t value) override;

    void setUniform(uniform_name_t property, Pr::vec2 value) override;
    void setUniform(uniform_name_t property, Pr::vec3 value) override;
    void setUniform(uniform_name_t property, Pr::vec4 value) override;
    void setUniform(uniform_name_t property, Pr::mat4 value) override;

    void setMat4(uniform_name_t property, float* value);

    OpenGLShader& setShader(const AssetArg& shaderPath, ShaderStage type);
    OpenGLShader& setShader(const char* data, ShaderStage type);

    void linkShaderProgram();

    [[nodiscard]] GLuint getShaderProgram() const { return shaderProgram_; }

   private:
    GLuint shaderProgram_;
    GLuint vertexShader_{INVALID_SHADER_ID};
    GLuint fragmentShader_{INVALID_SHADER_ID};

    std::map<shader_property_key_t, GLint> propertyMap_;
};

class OpenGLVAO {
    using attribute_offset_t = Pr::int64_t;

   public:
    OpenGLVAO(OpenGLBuffer* vertexBuffer, OpenGLBuffer* indexBuffer,
              const PipelineStructure& structure);
    OpenGLVAO();
    // OpenGLVAO();
    ~OpenGLVAO();

    void bind() const;

    OpenGLVAO& setAttribs(const PipelineStructure&);
    void finalise();
    [[nodiscard]] bool finalised() const { return finalised_; }

    OpenGLVAO(const OpenGLVAO&) = delete;
    OpenGLVAO& operator=(const OpenGLVAO&) = delete;
    OpenGLVAO(OpenGLVAO&&) = delete;
    OpenGLVAO& operator=(OpenGLVAO&&) = delete;

   private:
    void setAttribPointer(const PipelineAttribute& attribute, GLsizei stride,
                          attribute_offset_t offset);

    bool finalised_{false};

    GLuint vao_{};
};

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

void OpenGLShader::setUniform(uniform_name_t property, Pr::mat4 value) {
    setMat4(property, glm::gtc::value_ptr(value));
};

void OpenGLShader::setUniform(uniform_name_t property, Pr::float32_t value) {
    glUniform1f(propertyMap_[property], value);
};

void OpenGLShader::setUniform(uniform_name_t property, Pr::int32_t value) {
    glUniform1i(propertyMap_[property], value);
};

void OpenGLShader::setUniform(uniform_name_t property, Pr::uint32_t value) {
    glUniform1ui(propertyMap_[property], value);
};

void OpenGLShader::setUniform(uniform_name_t property, Pr::vec2 value) {
    glUniform2fv(propertyMap_[property], 1, glm::gtc::value_ptr(value));
};

void OpenGLShader::setUniform(uniform_name_t property, Pr::vec3 value) {
    glUniform3fv(propertyMap_[property], 1, glm::gtc::value_ptr(value));
};
void OpenGLShader::setUniform(uniform_name_t property, Pr::vec4 value) {
    glUniform4fv(propertyMap_[property], 1, glm::gtc::value_ptr(value));
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

OpenGLVAO::OpenGLVAO() {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
};

OpenGLVAO::OpenGLVAO(OpenGLBuffer* vertexBuffer, OpenGLBuffer* indexBuffer,
                     const PipelineStructure& structure)
    : OpenGLVAO() {
    if (vertexBuffer == nullptr) {
        Pr::CoreLog(
            ERROR,
            "OpenGLVAO received a null vertex buffer. Perhaps a dynamic cast "
            "failed?");
    } else {
        vertexBuffer->bind();
    }

    if (indexBuffer == nullptr) {
        Pr::CoreLog(
            ERROR,
            "OpenGLVAO received a null index buffer. Perhaps a dynamic cast "
            "failed?");
    } else {
        indexBuffer->bind();
    }

    this->setAttribs(structure);
    this->finalise();
}

OpenGLVAO::~OpenGLVAO() { glDeleteVertexArrays(1, &vao_); }

void OpenGLVAO::bind() const {
    Pr::CoreAssert(finalised_,
                   "An OpenGLVAO must be finalised before it can be bound.");

    glBindVertexArray(vao_);
};

void OpenGLVAO::finalise() {
    if (this->finalised()) {
        Pr::CoreLog(
            WARN,
            "An OpenGLVAO has been finalised a second time. Ignoring this "
            "request, and continuing with execution.");
        return;
    }
    finalised_ = true;

    // Unbind the VAO to make sure it doesn't have anything else written into it
    glBindVertexArray(0);
};

constexpr auto getGLAttributeDetails(ShaderDataType type) {
    struct {
        GLint count;
        GLenum gl_type;
    } vals{};

#define SWITCH_CASE(type)                                         \
    case type:                                                    \
        vals.gl_type = OpenGLTypeOf<(type)>;                      \
        vals.count = ShaderDataTypeTraits<(type)>::subtype_count; \
        break;

    switch (type) {
        SWITCH_CASE(ShaderDataType::SHORT);
        SWITCH_CASE(ShaderDataType::USHORT);
        SWITCH_CASE(ShaderDataType::INT);
        SWITCH_CASE(ShaderDataType::UINT);
        SWITCH_CASE(ShaderDataType::FLOAT);
        SWITCH_CASE(ShaderDataType::DOUBLE);
        SWITCH_CASE(ShaderDataType::VEC2);
        SWITCH_CASE(ShaderDataType::DVEC2);
        SWITCH_CASE(ShaderDataType::VEC3);
        SWITCH_CASE(ShaderDataType::DVEC3);
        SWITCH_CASE(ShaderDataType::VEC4);
        SWITCH_CASE(ShaderDataType::DVEC4);
        SWITCH_CASE(ShaderDataType::MAT3);
        SWITCH_CASE(ShaderDataType::DMAT3);
        SWITCH_CASE(ShaderDataType::MAT4);
        SWITCH_CASE(ShaderDataType::DMAT4);
    }

#undef SWITCH_CASE

    return vals;
}

void OpenGLVAO::setAttribPointer(const PipelineAttribute& attribute,
                                 GLsizei stride, attribute_offset_t offset) {
    auto x{getGLAttributeDetails(attribute.type)};

    glVertexAttribPointer(attribute.layout, x.count, x.gl_type, GL_FALSE,
                          stride, (void*)offset);
};

OpenGLVAO& OpenGLVAO::setAttribs(const PipelineStructure& structure) {
    if (this->finalised()) {
        Pr::CoreLog(
            ERROR,
            "Unable to set the attributes of an OpenGLVAO once it has been "
            "finalised.");
        return *this;
    }

    auto stride{static_cast<GLsizei>(structure.stride())};

    for (const PipelineAttribute& pipeline_attribute : structure.attributes) {
        // Get the offset of the attribute from the meshcontext

        // Enable the layout slot, then set its format
        glEnableVertexAttribArray(pipeline_attribute.layout);
        setAttribPointer(
            pipeline_attribute, stride,
            static_cast<attribute_offset_t>(pipeline_attribute.offset));
    }

    return *this;
}

}  // namespace Pr
