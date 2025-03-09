#include "Presto/Rendering/TextureInstance.h"

#include "Modules/RenderingManager.h"

namespace Presto {

TextureInstance::TextureInstance() {
    buffer_ = RenderingManager::get().createUniformBuffer(sizeof(uniforms_));
    this->updateBuffer();
};

void TextureInstance::updateBuffer() { buffer_->writeAny(this->uniforms_); };

}  // namespace Presto
