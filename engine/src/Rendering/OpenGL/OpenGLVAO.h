#pragma once

#include <GL/glew.h>
#include "Presto/Rendering/PipelineTypes.h"

namespace Presto {
class OpenGLBuffer;
class OpenGLPipeline;
struct VertexAttribute;

class OpenGLVAO {
   public:
    OpenGLVAO(OpenGLBuffer* vertexBuffer, OpenGLBuffer* indexBuffer,
              const OpenGLPipeline& pipeline);
    OpenGLVAO();

    // OpenGLVAO();
    ~OpenGLVAO();

    void bind() const;

    OpenGLVAO& setAttribs(const OpenGLPipeline&);
    void finalise();
    [[nodiscard]] bool finalised() const { return finalised_; }

   private:
    void setAttribPointer(const PipelineAttribute& attribute, GLsizei stride,
                          Presto::size_t offset);

    bool finalised_{false};

    GLuint vao_{};
};

};  // namespace Presto
