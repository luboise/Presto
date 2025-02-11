#pragma once

#include <GL/glew.h>

#include "Presto/Rendering/UniformBuffer.h"

namespace Presto {
class OpenGLUniformBuffer final : public UniformBuffer {
   public:
    explicit OpenGLUniformBuffer(Presto::size_t bufferSize);

    void bind(Presto::size_t index) override;
    void unbind() override;

    void write(const ByteArray& bytes, Presto::size_t offset = 0) override;

    ~OpenGLUniformBuffer();

   private:
    GLuint buffer_{};
};
}  // namespace Presto
