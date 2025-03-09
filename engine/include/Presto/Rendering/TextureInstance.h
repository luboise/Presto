#pragma once

#include "Presto/Aliases/Handles.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Utils/LazyCalculator.h"

namespace Presto {

class Texture;
class UniformBuffer;

class TextureInstance {
   public:
    TextureInstance();

    TextureInstance& setTexture(const TexturePtr& texture) {
        this->texture_ = texture;
        setDirty();

        return *this;
    };

    TextureInstance& setPosition(decltype(CanvasItemAttributes::position) pos) {
        uniforms_.position = pos;
        setDirty();

        return *this;
    }

    TextureInstance& setOffset(decltype(CanvasItemAttributes::offset) offset) {
        uniforms_.offset = offset;
        setDirty();

        return *this;
    }

    TextureInstance& setScale(decltype(CanvasItemAttributes::scale) scale) {
        uniforms_.scale = scale;
        setDirty();

        return *this;
    }

   private:
    Ptr<Texture> texture_;

    CanvasItemAttributes uniforms_{};
    Allocated<UniformBuffer> buffer_;

    void updateBuffer();
};

}  // namespace Presto
