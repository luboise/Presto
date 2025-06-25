export module presto.internal.rendering.opengl:texture;

import presto.internal.rendering:texture;

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

class OpenGLTexture2D final : public Texture2D {
   public:
    OpenGLTexture2D(opengl_dim_t width, opengl_dim_t height);

    void bind(uint8_t slot) override;

    void setImage(const ImagePtr& ptr) override;

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

}  // namespace Pr
