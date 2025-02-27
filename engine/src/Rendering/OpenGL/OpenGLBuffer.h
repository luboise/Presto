#pragma once

#include <GL/glew.h>

#include "Presto/Types/CoreTypes.h"

#include "OpenGLTypes.h"
#include "Rendering/Buffer.h"

namespace Presto {

struct OpenGLBufferDetails {
    std::int32_t draw_mode{};

    AttributeSet attributes{};
    GLsizei vertex_count{};
    GLsizei index_count{};
    GLint first_index{0};

    OpenGLBufferDetails() = default;
    OpenGLBufferDetails(const OpenGLBufferDetails& other) = delete;
};

struct OpenGLBufferArgs {
    ByteArray bytes;
    std::size_t vertex_count;
    AttributeSet attributes;
};

class OpenGLBuffer final : public Buffer {
    using HandleType = GLuint;

   public:
    /*OpenGLBuffer(AttributeSet attributes, std::size_t vertexCount,*/
    /*             ByteArray vertexBytes, IndexList indices,*/
    /*             std::int32_t drawMode);*/
    /**/

    OpenGLBuffer(BufferType type, Presto::size_t size);
    OpenGLBuffer(BufferType type, const ByteArray& data);

    ~OpenGLBuffer() override;

    /*[[nodiscard]] const OpenGLBufferDetails& getBufferDetails() const {*/
    /*    return data_;*/
    /*}*/

    /*[[nodiscard]] const AttributeSet& getAttributes() const {*/
    /*    return data_.attributes;*/
    /*}*/

    void bind() override;

    void write(const ByteArray& data, Presto::size_t offset = 0) override;

   private:
    // Vertex, then Index
    // std::array<HandleType, 2> handles_{0};

    HandleType buffer_{};
    Presto::int32_t openGlBufferType_;

    // OpenGLBufferDetails data_;
};

}  // namespace Presto
