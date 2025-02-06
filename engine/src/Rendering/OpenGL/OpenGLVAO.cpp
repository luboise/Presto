#include "OpenGLVAO.h"

#include "Rendering/OpenGL/OpenGLBuffer.h"
#include "Rendering/OpenGL/OpenGLPipeline.h"
#include "VertexAttribute.h"

#include "OpenGLTypes.h"

namespace Presto {
OpenGLVAO::OpenGLVAO(OpenGLBuffer& buffer, const OpenGLPipeline& pipeline) {
    // Vertex arrays
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Bind the buffers to the VAO
    buffer.bind();

    const auto& buffer_attributes{buffer.getAttributes()};

    Presto::size_t stride{buffer_attributes.stride()};

    for (const auto& pipeline_attribute :
         pipeline.getAttributes().getAttributes() | std::views::values) {
        const auto* buffer_attribute{
            buffer_attributes.getAttribute(pipeline_attribute.name)};
        PR_ASSERT(buffer_attribute != nullptr,
                  std::format("Attribute {} is missing in OpenGL buffer.",
                              pipeline_attribute.name));

        // Suppose we want to map input (layout=1)pos to (layout=7) pos

        // Enable the correct attribute (7) in the pipeline
        glEnableVertexAttribArray(pipeline_attribute.index);

        // Get the offset of the attribute from the meshcontext
        Presto::size_t offset{buffer_attribute->offset};

        setAttribPointer(pipeline_attribute, static_cast<GLsizei>(stride),
                         offset);
    }

    /*
// Set up attribute 1 (colour) from vbo
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, VEC_3, GL_FLOAT, GL_FALSE, STRIDE,
                      (void*)offsetof(Vertex, colour));

// Set up attribute 2 (normal) from vbo
glEnableVertexAttribArray(2);
glVertexAttribPointer(2, VEC_3, GL_FLOAT, GL_FALSE, STRIDE,
                      (void*)offsetof(Vertex, normal));

// Set up attribute 3 (texture coordinates) from vbo
glEnableVertexAttribArray(3);
glVertexAttribPointer(3, VEC_2, GL_FLOAT, GL_FALSE, STRIDE,
                      (void*)offsetof(Vertex, tex_coords));
                                              */
}

OpenGLVAO::~OpenGLVAO() { glDeleteVertexArrays(1, &vao_); }

void OpenGLVAO::bind() const { glBindVertexArray(vao_); };

void OpenGLVAO::setAttribPointer(const VertexAttribute& attribute,
                                 GLsizei stride, Presto::size_t offset) {
    auto convertedGlType{getGLAttribType(attribute.typeDetails.sub_type)};

    glVertexAttribPointer(
        attribute.index,
        static_cast<GLsizei>(attribute.typeDetails.sub_type_count),
        convertedGlType, GL_FALSE, stride, &offset);
};
}  // namespace Presto
