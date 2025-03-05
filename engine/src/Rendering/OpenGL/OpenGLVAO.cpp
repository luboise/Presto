#include "OpenGLVAO.h"

#include "Presto/Rendering/AttributeTypes.h"
#include "Presto/Rendering/ShaderTypes.h"
#include "Presto/Types/CoreTypeTraits.h"
#include "Rendering/OpenGL/OpenGLBuffer.h"
#include "Rendering/OpenGL/OpenGLPipeline.h"

#include "OpenGLTypes.h"
#include "Rendering/OpenGL/OpenGLTypeConversion.h"

namespace Presto {

OpenGLVAO::OpenGLVAO() {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
};

OpenGLVAO::OpenGLVAO(OpenGLBuffer* vertexBuffer, OpenGLBuffer* indexBuffer,
                     const PipelineStructure& structure)
    : OpenGLVAO() {
    if (vertexBuffer == nullptr) {
        PR_ERROR(
            "OpenGLVAO received a null vertex buffer. Perhaps a dynamic cast "
            "failed?");
    } else {
        vertexBuffer->bind();
    }

    if (indexBuffer == nullptr) {
        PR_ERROR(
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
    PR_CORE_ASSERT(finalised_,
                   "An OpenGLVAO must be finalised before it can be bound.");

    glBindVertexArray(vao_);
};

void OpenGLVAO::finalise() {
    if (this->finalised()) {
        PR_WARN(
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

#define SWITCH_CASE(type)                    \
    case type:                               \
        vals.gl_type = OpenGLTypeOf<(type)>; \
        vals.count = ShaderDataTypeTraits<(type)>::subtype_count;

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
        PR_ERROR(
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

}  // namespace Presto
