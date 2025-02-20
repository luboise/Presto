#pragma once

#include <GL/glew.h>

#include "Presto/Assets/Image.h"
#include "Presto/Rendering/Texture.h"

namespace Presto {
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
}  // namespace Presto
