#include "OpenGLBuffer.h"

namespace Presto {
OpenGLBuffer::OpenGLBuffer(AttributeSet attributes, std::size_t vertexCount,
                           ByteArray vertexBytes, IndexList indices,
                           std::int32_t drawMode) {
    auto bytes_expected{vertexCount * attributes.bytesRequired()};

    PR_CORE_ASSERT(
        vertexBytes.size() == bytes_expected,
        std::format("Mismatch between amount of bytes received ({}) and "
                    "amount of bytes expected ({} = {} vertices  x  {} "
                    "bytes/vertex)",
                    vertexBytes.size(), bytes_expected, vertexCount,
                    attributes.bytesRequired()));

    data_.attributes = std::move(attributes);

    data_.vertex_count = static_cast<GLsizei>(vertexCount);
    data_.index_count = static_cast<GLsizei>(indices.size());
    data_.first_index = 0;
    data_.draw_mode = drawMode;

    glGenBuffers(static_cast<GLsizei>(handles_.size()), handles_.data());

    // Create vertex buffer and write into it
    glGenBuffers(2, handles_.data());
    glBindBuffer(GL_ARRAY_BUFFER, handles_[0]);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexBytes.size()),
                 vertexBytes.data(), GL_STATIC_DRAW);

    // Bind the indices buffer (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(data_.index_count *
                                         sizeof(decltype(indices)::value_type)),
                 indices.data(), GL_STATIC_DRAW);
}

void OpenGLBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, handles_[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles_[1]);
}

OpenGLBuffer::~OpenGLBuffer() { glDeleteBuffers(2, handles_.data()); };
}  // namespace Presto
