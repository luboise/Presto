#include <span>

#include "presto/internal/rendering.h"

namespace Presto {

class OpenGLBuffer final : public Buffer {
    using HandleType = GLuint;

   public:
    OpenGLBuffer(BufferType type, Presto::size_t size);
    OpenGLBuffer(BufferType type, ByteArray& data);

    ~OpenGLBuffer() override;

    void bind() override;
    void write(buffer_write_t data, Presto::size_t offset = 0) override;

   private:
    HandleType buffer_{};
    Presto::int32_t openGlBufferType_;
};

class OpenGLUniformBuffer final : public UniformBuffer {
   public:
    explicit OpenGLUniformBuffer(Presto::size_t bufferSize);
    ~OpenGLUniformBuffer() override;

    void bind(Presto::size_t index) override;
    void unbind() override;

    void write(const std::span<const std::byte>& bytes,
               Presto::size_t offset = 0) override;

   private:
    GLuint buffer_{};
};

}  // namespace Presto
