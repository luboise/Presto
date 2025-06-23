module presto.internal.rendering.opengl;
import :buffer;
import presto.internal;

#include <span>

namespace Presto {

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
                                               : GL_DYNAMIC_DRAW};

    glBufferData(openGlBufferType_, static_cast<GLsizeiptr>(size), nullptr,
                 draw_type);
}

OpenGLBuffer::~OpenGLBuffer() { glDeleteBuffers(1, &buffer_); };

void OpenGLBuffer::write(buffer_write_t data, Presto::size_t offset) {
    if (data.size() + offset > this->size()) {
        PR_ERROR(
            "Writing {} bytes to a buffer of size {} would cause an overrun of "
            "size {}. Skipping this write.",
            data.size(), this->size(), data.size() + offset - this->size());
        return;
    }

    this->bind();

    // void* ptr{glMapBuffer(openGlBufferType_, GL_WRITE_ONLY)};
    void* ptr{glMapBufferRange(openGlBufferType_, static_cast<GLintptr>(offset),
                               static_cast<GLsizeiptr>(data.size()),
                               GL_MAP_WRITE_BIT)};
    std::memcpy(ptr, data.data(), data.size());
    glUnmapBuffer(openGlBufferType_);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() { glDeleteBuffers(1, &buffer_); };

OpenGLUniformBuffer::OpenGLUniformBuffer(Presto::size_t bufferSize)
    : UniformBuffer(bufferSize) {
    glGenBuffers(1, &buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_);

    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(this->size()),
                 nullptr, GL_DYNAMIC_DRAW);
};

void OpenGLUniformBuffer::write(const std::span<const std::byte>& bytes,
                                Presto::size_t offset) {
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(offset),
                    static_cast<GLsizeiptr>(bytes.size()), bytes.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
};

void OpenGLUniformBuffer::bind(Presto::size_t index) {
    glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(index), buffer_);
};
void OpenGLUniformBuffer::unbind() {};

}  // namespace Presto
