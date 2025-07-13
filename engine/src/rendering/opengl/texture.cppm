module;
#include <GL/glew.h>

export module presto.internal.rendering.opengl:texture;
import presto.internal.rendering;

import presto.assets.image;
import presto.rendering.texture;

import presto.core.logging;
import presto.core.assert;
import presto.types;

import std;

export namespace Pr {

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

/*
class OpenGLTexture : public Texture {
   public:
    explicit OpenGLTexture(const Pr::ImageData& image) { load(image); };

    ~OpenGLTexture() override;

    [[nodiscard]] bool isLoaded() const {
        return textureId_ != 0 && samplerId_ != 0;
    };

    void load(const Pr::ImageData& data);
    void unload();

    void bind(Pr::uint8_t slot) override;

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
*/

class OpenGLTexture2D final : public Texture2D {
   public:
    OpenGLTexture2D(opengl_dim_t width, opengl_dim_t height);

    void bind(uint8_t slot) override;

    void setImage(const Pr::Ptr<Pr::ImageAsset>& ptr) override;

    void load() override;

    [[nodiscard]] Pr::size_t height() const override;
    [[nodiscard]] Pr::size_t width() const override;

    void write(ByteArray bytes) override;

   private:
    opengl_dim_t width_{0};
    opengl_dim_t height_{0};

    OpenGLTextureInstance textureInst_;
    OpenGLSamplerInstance samplerInst_;

    void reloadInstances();
};

class OpenGLTextureFactory final : public TextureFactory {
   public:
    OpenGLTextureFactory() = default;

    [[nodiscard]] FactoryOutputType<Texture2D> new2D(
        texture_extents_t width, texture_extents_t height) override {
        return std::make_shared<OpenGLTexture2D>(
            static_cast<opengl_dim_t>(width),
            static_cast<opengl_dim_t>(height));
    };
};

/*
void OpenGLTexture::load(const Pr::Image& image) {
    imageWidth_ = image.width;
    imageHeight_ = image.height;

    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(imageWidth_),
                 static_cast<GLsizei>(imageHeight_), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image.bytes.data());
    glGenerateTextureMipmap(textureId_);

    glGenSamplers(1, &samplerId_);

    // TODO: Make these parameters in the constructor
    glSamplerParameteri(samplerId_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(samplerId_, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(samplerId_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(samplerId_, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void OpenGLTexture::unload() {
    if (!this->isLoaded()) {
        return;
    }

    glDeleteTextures(1, &textureId_);
    glDeleteSamplers(1, &samplerId_);
};

OpenGLTexture::~OpenGLTexture() { this->unload(); }

void OpenGLTexture::bind(Pr::uint8_t slot) const {
    constexpr auto MAX_SLOT = 31;

    Pr::CoreAssert(
        slot <= 31,
        std::format("Texture units must be between 0 and {} (received {}).",
                    MAX_SLOT, slot));

    auto slot_address = GL_TEXTURE0 + slot;

    glActiveTexture(slot_address);
    glBindTexture(GL_TEXTURE_2D, textureId_);

    glBindSampler(slot, samplerId_);
}
OpenGLTexture& OpenGLTexture::operator=(OpenGLTexture&& other) noexcept {
    imageWidth_ = other.imageWidth_;
    imageHeight_ = other.imageHeight_;
    textureId_ = other.textureId_;
    samplerId_ = other.samplerId_;
    other.imageWidth_ = 0;
    other.imageHeight_ = 0;
    other.textureId_ = 0;
    other.samplerId_ = 0;

    return *this;
}
OpenGLTexture::OpenGLTexture(OpenGLTexture& other)
    : imageWidth_(other.imageWidth_),
      imageHeight_(other.imageHeight_),
      textureId_(other.textureId_),
      samplerId_(other.samplerId_) {
    other.imageWidth_ = 0;
    other.imageHeight_ = 0;
    other.textureId_ = 0;
    other.samplerId_ = 0;
};
OpenGLTexture::OpenGLTexture(OpenGLTexture&& other) noexcept
    : imageWidth_(other.imageWidth_),
      imageHeight_(other.imageHeight_),
      textureId_(other.textureId_),
      samplerId_(other.samplerId_) {
    other.imageWidth_ = 0;
    other.imageHeight_ = 0;
    other.textureId_ = 0;
    other.samplerId_ = 0;
};
*/

void OpenGLTexture2D::setImage(const Pr::Ptr<Pr::ImageAsset>& ptr) {};

void OpenGLTexture2D::bind(uint8_t slot) {
    glActiveTexture(GL_TEXTURE0 + static_cast<unsigned int>(slot));
    glBindTexture(GL_TEXTURE_2D, textureInst_);
}
void OpenGLTexture2D::load() {
    if (loaded()) {
        Pr::CoreLog(WARN,
                    "Attempting to reload an existing image {}. Ignoring this "
                    "request.",
                    this->id());
        return;
    }
    if (!this->image()) {
        Pr::CoreLog(WARN, "No image available. Returning.");
        return;
    }

    if (width_ != static_cast<opengl_dim_t>(image()->width()) ||
        height_ != static_cast<opengl_dim_t>(image()->height())) {
        reloadInstances();
    }
}

OpenGLTexture2D::OpenGLTexture2D(opengl_dim_t width, opengl_dim_t height)
    : width_(width), height_(height) {
    reloadInstances();
}

Pr::size_t OpenGLTexture2D::width() const { return width_; }
Pr::size_t OpenGLTexture2D::height() const { return height_; }

void OpenGLTexture2D::reloadInstances() {
    // Update the height and width if they have been changed
    if (const auto& img{image()}; img != nullptr) {
        width_ = static_cast<opengl_dim_t>(img->width());
        height_ = static_cast<opengl_dim_t>(img->height());
    }

    // Bind this texture to the last slot to preserve the state of the
    // first 30
    this->bind(31);
    glTexImage2D(GL_TEXTURE_2D, 0, OPENGL_PIXEL_FORMAT, width_, height_, 0,
                 OPENGL_PIXEL_FORMAT, OPENGL_TEXTURE_DATA_TYPE, nullptr);

    // TODO: Make this based on the image being used
    glSamplerParameteri(samplerInst_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(samplerInst_, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(samplerInst_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(samplerInst_, GL_TEXTURE_WRAP_T, GL_REPEAT);
};

void OpenGLTexture2D::write(ByteArray bytes) {
    if (auto size{pixelCount() * OPENGL_PIXEL_SIZE}; bytes.size() != size) {
        Pr::CoreLog(ERROR,
                    "The number of bytes received ({}) does not match the "
                    "number "
                    "of bytes expected ({}). Skipping this texture write.",
                    size, bytes.size());
        return;
    }

    this->bind(31);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_,
                    OPENGL_PIXEL_FORMAT, OPENGL_TEXTURE_DATA_TYPE,
                    bytes.data());
    glGenerateTextureMipmap(textureInst_);
}

}  // namespace Pr
