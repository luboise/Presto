#include "OpenGLVAO.h"

#include "Presto/Rendering/AttributeTypes.h"
#include "Rendering/OpenGL/OpenGLBuffer.h"
#include "Rendering/OpenGL/OpenGLPipeline.h"

#include "OpenGLTypes.h"

namespace Presto {

OpenGLVAO::OpenGLVAO() {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
};

OpenGLVAO::OpenGLVAO(OpenGLBuffer* vertexBuffer, OpenGLBuffer* indexBuffer,
                     const OpenGLPipeline& pipeline)
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

    this->setAttribs(pipeline);
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

void OpenGLVAO::setAttribPointer(const PipelineAttribute& attribute,
                                 GLsizei stride, Presto::size_t offset) {
    AttributeTypeDetails type_details{attributeTypeDetailsOf(attribute.type)};

    glVertexAttribPointer(
        attribute.layout, static_cast<GLsizei>(type_details.sub_type_count),
        getGLAttribType(type_details.sub_type), GL_FALSE, stride, &offset);
};

OpenGLVAO& OpenGLVAO::setAttribs(const OpenGLPipeline& pipeline) {
    if (this->finalised()) {
        PR_ERROR(
            "Unable to set the attributes of an OpenGLVAO once it has been "
            "finalised.");
        return *this;
    }

    const auto& structure{pipeline.getStructure()};

    const std::vector<PipelineAttribute>& pipeline_attributes{
        pipeline.getAttributes()};

    auto stride{static_cast<GLsizei>(structure.stride())};

    for (const PipelineAttribute& pipeline_attribute : pipeline_attributes) {
        // Get the offset of the attribute from the meshcontext

        // Enable the layout slot, then set its format
        glEnableVertexAttribArray(pipeline_attribute.layout);
        setAttribPointer(pipeline_attribute, stride, pipeline_attribute.offset);
    }

    return *this;
}

}  // namespace Presto
