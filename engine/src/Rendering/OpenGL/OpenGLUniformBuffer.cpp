#include <GL/glew.h>

#include "Rendering/OpenGL/OpenGLUniformBuffer.h"

namespace Presto {

OpenGLUniformBuffer::~OpenGLUniformBuffer() { glDeleteBuffers(1, &buffer_); };

OpenGLUniformBuffer::OpenGLUniformBuffer(Presto::size_t bufferSize)
    : UniformBuffer(bufferSize) {
    glGenBuffers(1, &buffer_);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(this->size()),
                 nullptr, GL_DYNAMIC_DRAW);
};

void OpenGLUniformBuffer::write(const std::span<std::byte>& bytes,
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
