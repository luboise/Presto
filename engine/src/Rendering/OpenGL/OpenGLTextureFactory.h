#include "Rendering/OpenGL/Abstractions/OpenGLTextures.h"

#include "Rendering/Renderer.h"

namespace Presto {

class OpenGLTextureFactory final : public TextureFactory {
   public:
    OpenGLTextureFactory() = default;

    [[nodiscard]] Allocated<Texture2D> new2D(
        texture_extents_t width, texture_extents_t height) override {
        return std::make_unique<OpenGLTexture2D>(
            static_cast<opengl_dim_t>(width),
            static_cast<opengl_dim_t>(height));
    };
};

}  // namespace Presto
