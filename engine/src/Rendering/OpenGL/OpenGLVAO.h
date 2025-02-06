#pragma once

#include <GL/glew.h>

namespace Presto {
class OpenGLBuffer;
class OpenGLPipeline;
struct VertexAttribute;

class OpenGLVAO {
   public:
    OpenGLVAO(OpenGLBuffer& buffer, const OpenGLPipeline& pipeline);
    ~OpenGLVAO();

    void bind() const;

   private:
    void setAttribPointer(const VertexAttribute& attribute, GLsizei stride,
                          Presto::size_t offset);

    GLuint vao_{};
};

};  // namespace Presto
