#include "presto/objects/components/canvas.h"
import :canvasitems;

// export module presto.objects.components:canvas;

namespace Presto {
struct CanvasItem::Impl {
    StructUniformBuffer<CanvasItemAttributes> struct_buffer{};
    CanvasItemAttributes attributes{};

    mesh_registration_id_t mesh_id{PR_MESH_QUAD};

    // CanvasDrawDetails draw_details;

    TexturePtr texture;
};

CanvasItem& CanvasItem::setTexture(const TexturePtr& texture) {
    impl_->texture = texture;

    return *this;
};

CanvasItem::CanvasItem() : LazyCalculator() {
    impl_ = std::make_unique<Impl>();
};

CanvasItem::CanvasItem(vec2 position) : CanvasItem() {
    impl_ = std::make_unique<Impl>();
    impl_->struct_buffer.write({.position = position});
}

CanvasItem::CanvasItem(CanvasItemAttributes attributes) : CanvasItem() {
    impl_->attributes = attributes;
    impl_->struct_buffer.write(impl_->attributes);
};

const TexturePtr& CanvasItem::texture() const { return impl_->texture; };

const CanvasPosition& CanvasItem::position() const {
    return impl_->struct_buffer.data().position;
};

CanvasItem& CanvasItem::setAttributes(CanvasItemAttributes attributes) {
    impl_->attributes = attributes;
    setDirty();

    return *this;
};

CanvasItem::~CanvasItem() = default;
CanvasItem::CanvasItem(CanvasItem&&) noexcept = default;

// void CanvasItem::setDrawDetails(CanvasDrawDetails details) {
//     impl_->draw_details = details;
// };
//
// const CanvasDrawDetails& CanvasItem::drawDetails() const {
//     return impl_->draw_details;
// };

[[nodiscard]] mesh_registration_id_t CanvasItem::meshId() const {
    return impl_->mesh_id;
};

void CanvasItem::setMeshId(mesh_registration_id_t id) { impl_->mesh_id = id; };

/*
void CanvasRect::setRectangle(Rectangle rect) {
    rect_ = rect;

    setAttributes(
        {.position{rect.bottom_left}, .scale{rect.width(), rect.height()}});
};

const Rectangle& CanvasRect::rectangle() const { return rect_; };
*/

UniformBuffer& CanvasItem::buffer() {
    if (isDirty()) {
        impl_->struct_buffer.write(impl_->attributes);
        setDirty(false);
    }
    return impl_->struct_buffer.buffer();
}

CanvasRect::CanvasRect(CanvasRectProps props) {
    setMeshId(PR_MESH_QUAD);

    this->setProps(props);
};

CanvasItem& CanvasItem::operator=(CanvasItem&&) noexcept = default;

void CanvasRect::setProps(CanvasRectProps props) {
    CanvasItemAttributes new_attribs{this->attributes()};

    new_attribs.position = props.pos;
    new_attribs.scale = {props.width, props.height};
    setAttributes(new_attribs);
};

CanvasItemAttributes CanvasItem::attributes() const {
    return impl_->attributes;
};

CanvasItem& CanvasItem::setOpacity(Presto::float32_t opacity) {
    impl_->attributes.opacity = std::clamp(opacity, 0.F, 1.F);
    setDirty();

    return *this;
}

CanvasButton::CanvasButton(CanvasPosition position)
    : CanvasItem(position) {

      };

void CanvasButton::setImage(const ImagePtr& image) { image_ = image; };
}  // namespace Presto
