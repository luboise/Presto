#include "Presto/Objects/Components/UI/CanvasItem.h"
#include <memory>
#include "Presto/Rendering/RenderTypes.h"
#include "Rendering/StructBuffer.h"

namespace Presto {

struct CanvasItem::Impl {
    StructUniformBuffer<CanvasItemAttributes> struct_buffer;
    TexturePtr texture;
};

CanvasItem& CanvasItem::setTexture(const TexturePtr& texture) {
    impl_->texture = texture;

    return *this;
};

CanvasItem::CanvasItem(vec2 position) : LazyCalculator() {
    impl_ = std::make_unique<Impl>();
    impl_->struct_buffer.write({.position = position});
}

const TexturePtr& CanvasItem::texture() const { return impl_->texture; };

const CanvasPosition& CanvasItem::position() const {
    return impl_->struct_buffer.data().position;
};

CanvasItem& CanvasItem::setAttributes(CanvasItemAttributes attributes) {
    impl_->struct_buffer.write(attributes);

    return *this;
};

CanvasItem::~CanvasItem() = default;
CanvasItem::CanvasItem(CanvasItem&&) noexcept = default;
}  // namespace Presto
