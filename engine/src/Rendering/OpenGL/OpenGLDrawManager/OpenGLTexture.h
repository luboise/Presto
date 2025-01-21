#include <GL/gl.h>
#include "Presto/Resources/Image.h"

namespace Presto {
    class OpenGLTexture {
       public:
        OpenGLTexture() = default;
        OpenGLTexture& operator=(const OpenGLTexture&) = delete;
        OpenGLTexture& operator=(OpenGLTexture&& other) noexcept {
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

        explicit OpenGLTexture(const Presto::Image& image) { load(image); };

        OpenGLTexture(OpenGLTexture& other)
            : imageWidth_(other.imageWidth_),
              imageHeight_(other.imageHeight_),
              textureId_(other.textureId_),
              samplerId_(other.samplerId_) {
            other.imageWidth_ = 0;
            other.imageHeight_ = 0;
            other.textureId_ = 0;
            other.samplerId_ = 0;
        };

        OpenGLTexture(OpenGLTexture&& other) noexcept
            : imageWidth_(other.imageWidth_),
              imageHeight_(other.imageHeight_),
              textureId_(other.textureId_),
              samplerId_(other.samplerId_) {
            other.imageWidth_ = 0;
            other.imageHeight_ = 0;
            other.textureId_ = 0;
            other.samplerId_ = 0;
        };

        ~OpenGLTexture();

        [[nodiscard]] bool isLoaded() const {
            return textureId_ != 0 && samplerId_ != 0;
        };

        void load(const Presto::Image& image);
        void unload();

        void bind(std::uint8_t slot) const;

       private:
        size_t imageWidth_{0};
        size_t imageHeight_{0};

        GLuint textureId_{0};
        GLuint samplerId_{0};
    };
}  // namespace Presto
