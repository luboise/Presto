module;
#include <GL/glew.h>

export module presto.internal.rendering.opengl:buffer;

import presto.internal.rendering;

import presto.core.assert;
import presto.core.logging;

import std;

export namespace Pr {

class OpenGLBuffer final : public Buffer {
    using HandleType = GLuint;

   public:
    OpenGLBuffer(BufferType type, Pr::size_t size) : Buffer(type, size) {
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
                Pr::CoreAssert(false, "Invalid buffer type");
                return;
        }

        // Create vertex buffer and write into it
        glGenBuffers(1, &buffer_);
        glBindBuffer(openGlBufferType_, buffer_);

        auto draw_type{type == BufferType::UNIFORM ? GL_DYNAMIC_DRAW
                                                   : GL_STATIC_DRAW};

        glBufferData(openGlBufferType_, static_cast<GLsizeiptr>(size), nullptr,
                     draw_type);
    };

    OpenGLBuffer(BufferType type, ByteArray& data)
        : OpenGLBuffer(type, data.size()) {
        this->write(data, 0);
    };

    ~OpenGLBuffer() override { glDeleteBuffers(1, &buffer_); };

    void bind() override { glBindBuffer(openGlBufferType_, buffer_); };
    void write(buffer_write_t data, Pr::size_t offset = 0) override {
        if (data.size() + offset > this->size()) {
            Pr::Log(ERROR,
                    "Writing {} bytes to a buffer of size {} would cause an "
                    "overrun of "
                    "size {}. Skipping this write.",
                    data.size(), this->size(),
                    data.size() + offset - this->size());
            return;
        }

        this->bind();

        // void* ptr{glMapBuffer(openGlBufferType_, GL_WRITE_ONLY)};
        void* ptr{glMapBufferRange(
            openGlBufferType_, static_cast<GLintptr>(offset),
            static_cast<GLsizeiptr>(data.size()), GL_MAP_WRITE_BIT)};
        std::memcpy(ptr, data.data(), data.size());
        glUnmapBuffer(openGlBufferType_);
    };

   private:
    HandleType buffer_{};
    Pr::int32_t openGlBufferType_;
};

class OpenGLUniformBuffer final : public UniformBuffer {
   public:
    explicit OpenGLUniformBuffer(Pr::size_t bufferSize)
        : UniformBuffer(bufferSize) {
        glGenBuffers(1, &buffer_);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer_);

        glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(this->size()),
                     nullptr, GL_DYNAMIC_DRAW);
    }

    ~OpenGLUniformBuffer() override;

    void write(const std::span<const std::byte>& bytes,
               Pr::size_t offset) override {
        glBindBuffer(GL_UNIFORM_BUFFER, buffer_);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(offset),
                        static_cast<GLsizeiptr>(bytes.size()), bytes.data());
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    };

    void bind(Pr::size_t index) override {
        glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(index),
                         buffer_);
    };
    void unbind() override {};

   private:
    GLuint buffer_{};
};

}  // namespace Pr
