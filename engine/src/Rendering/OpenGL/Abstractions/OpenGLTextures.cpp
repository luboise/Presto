#include "OpenGLTextures.h"

namespace Presto {

void OpenGLTexture2D::setImage(const ImagePtr& ptr) {};

void OpenGLTexture2D::bind(uint8_t slot) {
    glActiveTexture(GL_TEXTURE0 + static_cast<unsigned int>(slot));
    glBindTexture(GL_TEXTURE_2D, textureInst_);
}
void OpenGLTexture2D::load() {
    if (loaded()) {
        PR_CORE_WARN(
            "Attempting to reload an existing image {}. Ignoring this "
            "request.",
            this->id());
        return;
    }
    if (!this->image()) {
        PR_CORE_WARN("No image available. Returning.");
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

std::size_t OpenGLTexture2D::width() const { return width_; }
std::size_t OpenGLTexture2D::height() const { return height_; }

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
        PR_ERROR(
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

}  // namespace Presto
