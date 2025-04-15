#include "Presto/Objects/Components/RenderComponent.h"
#include "Presto/Objects/Components/Renderable/ModelSubcomponent.h"
#include "Presto/Objects/Components/Renderable/QuadSubcomponent.h"

namespace Presto {

struct RenderComponent::Impl {
    std::vector<QuadSubcomponent> quads;
    std::vector<ModelSubcomponent> models;
};

RenderComponent::RenderComponent() = default;
RenderComponent::~RenderComponent() = default;

void RenderComponent::onEnterScene() {
    // TODO: Fix
    for (ModelSubcomponent& model : impl_->models) {
        for (auto& draw : model.draws) {
            draw.mesh->ensureLoaded();
        }
    }

    // This might be needed later if selective material usage is implemented
    /*
for (auto& material : materialOverrides_) {
    if (material != nullptr) {
        material->ensureLoaded();
    }
}
    */
};

RenderComponent& RenderComponent::addQuad(const QuadSubcomponent& quad) {
    impl_->quads.emplace_back(quad);

    return *this;
};

RenderComponent& RenderComponent::addQuad(QuadSubcomponent&& quad) {
    impl_->quads.emplace_back(std::move(quad));

    return *this;
};

RenderComponent& RenderComponent::addModel(const ModelSubcomponent& model) {
    impl_->models.emplace_back(model);

    return *this;
};

RenderComponent& RenderComponent::addModel(ModelSubcomponent&& model) {
    impl_->models.emplace_back(std::move(model));

    return *this;
};

const std::vector<QuadSubcomponent>& RenderComponent::getQuads() const {
    return impl_->quads;
};

const std::vector<ModelSubcomponent>& RenderComponent::getModels() const {
    return impl_->models;
};

}  // namespace Presto
