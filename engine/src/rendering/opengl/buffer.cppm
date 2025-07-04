export module presto.internal.rendering.opengl:buffer;

import std;

import presto.internal.rendering;

export namespace Pr {

class OpenGLBuffer final : public Buffer {
    using HandleType = GLuint;

   public:
    OpenGLBuffer(BufferType type, Pr::size_t size);
    OpenGLBuffer(BufferType type, ByteArray& data);

    ~OpenGLBuffer() override;

    void bind() override;
    void write(buffer_write_t data, Pr::size_t offset = 0) override;

   private:
    HandleType buffer_{};
    Pr::int32_t openGlBufferType_;
};

class OpenGLUniformBuffer final : public UniformBuffer {
   public:
    explicit OpenGLUniformBuffer(Pr::size_t bufferSize);
    ~OpenGLUniformBuffer() override;

    void bind(Pr::size_t index) override;
    void unbind() override;

    void write(const std::span<const std::byte>& bytes,
               Pr::size_t offset = 0) override;

   private:
    GLuint buffer_{};
};

}  // namespace Pr
