#include "OpenGLBuffer.h"
#include "Presto/Core/Logging.h"

namespace Presto {

/**/
/*OpenGLBuffer::OpenGLBuffer(AttributeSet attributes, std::size_t vertexCount,*/
/*                           ByteArray vertexBytes, IndexList indices,*/
/*                           std::int32_t drawMode) {*/
/*    auto bytes_expected{vertexCount * attributes.bytesRequired()};*/
/**/
/*    PR_CORE_ASSERT(*/
/*        vertexBytes.size() == bytes_expected,*/
/*        std::format("Mismatch between amount of bytes received ({}) and "*/
/*                    "amount of bytes expected ({} = {} vertices  x  {} "*/
/*                    "bytes/vertex)",*/
/*                    vertexBytes.size(), bytes_expected, vertexCount,*/
/*                    attributes.bytesRequired()));*/
/**/
/*    data_.attributes = std::move(attributes);*/
/**/
/*    data_.vertex_count = static_cast<GLsizei>(vertexCount);*/
/*    data_.index_count = static_cast<GLsizei>(indices.size());*/
/*    data_.first_index = 0;*/
/*    data_.draw_mode = drawMode;*/
/**/
/*    glGenBuffers(static_cast<GLsizei>(handles_.size()), handles_.data());*/
/**/
/*    // Create vertex buffer and write into it*/
/*    glGenBuffers(2, handles_.data());*/
/*    glBindBuffer(GL_ARRAY_BUFFER, handles_[0]);*/
/*    glBufferData(GL_ARRAY_BUFFER,
 * static_cast<GLsizeiptr>(vertexBytes.size()),*/
/*                 vertexBytes.data(), GL_STATIC_DRAW);*/
/**/
/*    // Bind the indices buffer (EBO)*/
/*    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles_[1]);*/
/*    glBufferData(GL_ELEMENT_ARRAY_BUFFER,*/
/*                 static_cast<GLsizeiptr>(data_.index_count **/
/*                                         sizeof(decltype(indices)::value_type)),*/
/*                 indices.data(), GL_STATIC_DRAW);*/
/*}*/

void OpenGLBuffer::bind() { glBindBuffer(openGlBufferType_, buffer_); }

OpenGLBuffer::OpenGLBuffer(BufferType type, ByteArray& data)
    : OpenGLBuffer(type, data.size()) {
    this->write(data, 0);
};

OpenGLBuffer::OpenGLBuffer(BufferType type, Presto::size_t size)
    : Buffer(type, size) {
    switch (type) {
        case BufferType::VERTEX:
            openGlBufferType_ = GL_ARRAY_BUFFER;
            break;
        case BufferType::INDEX:
            openGlBufferType_ = GL_ELEMENT_ARRAY_BUFFER;
            break;
        case BufferType::UNIFORM:
            openGlBufferType_ = GL_UNIFORM_BUFFER;
            break;
        default:
            PR_CORE_ASSERT(false, "Invalid buffer type");
            return;
    }

    // Create vertex buffer and write into it
    glGenBuffers(1, &buffer_);
    glBindBuffer(openGlBufferType_, buffer_);

    auto draw_type{type == BufferType::UNIFORM ? GL_DYNAMIC_DRAW
                                               : GL_STATIC_DRAW};

    glBufferData(openGlBufferType_, static_cast<GLsizeiptr>(size), nullptr,
                 draw_type);
}

OpenGLBuffer::~OpenGLBuffer() { glDeleteBuffers(1, &buffer_); };

void OpenGLBuffer::write(buffer_write_t data, Presto::size_t offset) {
    if (offset >= data.size()) {
        PR_ERROR(
            "Offset {} is out of range for input data of size {}. Skipping "
            "this write.",
            offset, data.size());

        return;
    }

    auto write_size{getWriteSize(data.size(), offset)};

    this->bind();
    glBufferSubData(openGlBufferType_, static_cast<GLintptr>(offset),
                    static_cast<GLsizeiptr>(write_size), data.data());
}

}  // namespace Presto
