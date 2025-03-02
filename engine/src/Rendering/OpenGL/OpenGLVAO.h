#pragma once

#include <GL/glew.h>
#include "Presto/Rendering/PipelineTypes.h"

namespace Presto {
class OpenGLBuffer;
class OpenGLPipeline;
struct VertexAttribute;

class OpenGLVAO {
    using attribute_offset_t = std::int64_t;

   public:
    OpenGLVAO(OpenGLBuffer* vertexBuffer, OpenGLBuffer* indexBuffer,
              const PipelineStructure& structure);
    OpenGLVAO();
    // OpenGLVAO();
    ~OpenGLVAO();

    void bind() const;

    OpenGLVAO& setAttribs(const PipelineStructure&);
    void finalise();
    [[nodiscard]] bool finalised() const { return finalised_; }

    OpenGLVAO(const OpenGLVAO&) = delete;
    OpenGLVAO& operator=(const OpenGLVAO&) = delete;
    OpenGLVAO(OpenGLVAO&&) = delete;
    OpenGLVAO& operator=(OpenGLVAO&&) = delete;

   private:
    void setAttribPointer(const PipelineAttribute& attribute, GLsizei stride,
                          attribute_offset_t offset);

    bool finalised_{false};

    GLuint vao_{};
};

};  // namespace Presto
