#pragma once

#include <GL/glew.h>

#include "Presto/Types/CoreTypes.h"

#include "OpenGLTypes.h"
#include "Rendering/Buffer.h"

namespace Presto {

struct OpenGLBufferArgs {
    ByteArray bytes;
    std::size_t vertex_count;
    AttributeSet attributes;
};

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

}  // namespace Presto
