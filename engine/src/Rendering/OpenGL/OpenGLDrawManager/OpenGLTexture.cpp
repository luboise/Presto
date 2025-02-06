#include <GL/glew.h>

#include "OpenGLTexture.h"

namespace Presto {
void OpenGLTexture::load(const Presto::Image& image) {
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

void OpenGLTexture::bind(std::uint8_t slot) const {
    constexpr auto MAX_SLOT = 31;

    PR_CORE_ASSERT(
        slot <= 31,
        std::format("Texture units must be between 0 and {} (received {}).",
                    MAX_SLOT, slot));

    auto slot_address = GL_TEXTURE0 + slot;

    glActiveTexture(slot_address);
    glBindTexture(GL_TEXTURE_2D, textureId_);

    glBindSampler(slot, samplerId_);
}
}  // namespace Presto
