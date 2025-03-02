#pragma once

#include <GL/glew.h>

#include "Presto/Assets/Image.h"

#include "Presto/Rendering/TextureTypes.h"

namespace Presto {

constexpr int OPENGL_PIXEL_FORMAT{GL_RGBA};
constexpr int OPENGL_TEXTURE_DATA_TYPE{GL_UNSIGNED_BYTE};
constexpr int OPENGL_PIXEL_SIZE{4};
using opengl_dim_t = GLsizei;

/**
 * Utility class that holds a texture handle, and deallocates it when it goes
 * out of scope. This can be reused amongst the OpenGL Texture implementations
 */
class OpenGLTextureInstance {
   public:
    OpenGLTextureInstance() { glGenTextures(1, &id_); };
    ~OpenGLTextureInstance() { glDeleteTextures(1, &id_); }
    [[nodiscard]] GLuint id() const { return id_; }

    // Implicit conversion
    operator GLuint() const { return id_; }

   private:
    GLuint id_{};
};

/**
 * Utility class that holds a texture handle, and deallocates it when it goes
 * out of scope. This can be reused amongst the OpenGL Texture implementations
 */
class OpenGLSamplerInstance {
   public:
    OpenGLSamplerInstance() { glGenSamplers(1, &id_); };
    ~OpenGLSamplerInstance() { glDeleteSamplers(1, &id_); }
    [[nodiscard]] GLuint id() const { return id_; }

    // Implicit conversion
    operator GLuint() const { return id_; }

   private:
    GLuint id_{};
};

class OpenGLTexture : public Texture {
   public:
    explicit OpenGLTexture(const Presto::Image& image) { load(image); };

    ~OpenGLTexture() override;

    [[nodiscard]] bool isLoaded() const {
        return textureId_ != 0 && samplerId_ != 0;
    };

    void load(const Presto::Image& image);
    void unload();

    void bind(std::uint8_t slot) override;

    OpenGLTexture& operator=(const OpenGLTexture&) = delete;
    OpenGLTexture(OpenGLTexture& other);

    OpenGLTexture& operator=(OpenGLTexture&& other) noexcept;
    OpenGLTexture(OpenGLTexture&& other) noexcept;

   private:
    size_t imageWidth_{0};
    size_t imageHeight_{0};

    GLuint textureId_{0};
    GLuint samplerId_{0};
};

class OpenGLTexture2D final : public Texture2D {
   public:
    void bind(uint8_t slot) override {
        glActiveTexture(GL_TEXTURE0 + static_cast<unsigned int>(slot));
        glBindTexture(GL_TEXTURE_2D, textureInst_);
    }

    OpenGLTexture2D(opengl_dim_t width, opengl_dim_t height)
        : width_(width), height_(height) {
        this->bind(0);
        glTexImage2D(GL_TEXTURE_2D, 0, OPENGL_PIXEL_FORMAT, width_, height_, 0,
                     OPENGL_PIXEL_FORMAT, OPENGL_TEXTURE_DATA_TYPE, nullptr);

        // TODO: Parametrise this with a proper sampler value system
        glSamplerParameteri(samplerInst_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(samplerInst_, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
        glSamplerParameteri(samplerInst_, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(samplerInst_, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    [[nodiscard]] std::size_t height() const override { return height_; }
    [[nodiscard]] std::size_t width() const override { return width_; }

    void write(ByteArray bytes) override {
        if (auto size{pixelCount() * OPENGL_PIXEL_SIZE}; bytes.size() != size) {
            PR_ERROR(
                "The number of bytes received ({}) does not match the "
                "number "
                "of bytes expected ({}). Skipping this texture write.",
                size, bytes.size());
            return;
        }

        this->bind(0);

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_,
                        OPENGL_PIXEL_FORMAT, OPENGL_TEXTURE_DATA_TYPE,
                        bytes.data());
        glGenerateTextureMipmap(textureInst_);
    }

   private:
    opengl_dim_t width_{0};
    opengl_dim_t height_{0};

    OpenGLTextureInstance textureInst_;
    OpenGLSamplerInstance samplerInst_;
};

}  // namespace Presto
