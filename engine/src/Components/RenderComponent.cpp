#include "Presto/Objects/Components/RenderComponent.h"
#include "Presto/Objects/Components/Renderables/QuadSubcomponent.h"

#include "Presto/Assets/ModelAsset.h"  // IWYU pragma: export

namespace Presto {

struct RenderComponent::Impl {
    std::vector<QuadSubcomponent> quads;
    std::vector<ModelPtr> models;
};

RenderComponent::RenderComponent() { impl_ = std::make_unique<Impl>(); };
RenderComponent::~RenderComponent() = default;

void RenderComponent::onEnterScene() {
    for (ModelPtr& model : impl_->models) {
        model->ensureLoaded();
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

RenderComponent& RenderComponent::addModel(const ModelPtr& model) {
    if (model == nullptr) {
        PR_ERROR(
            "Unable to add a null model to a RenderComponent. Skipping this "
            "request.");
        return *this;
    }
    impl_->models.push_back(model);

    return *this;
};

std::vector<QuadSubcomponent>& RenderComponent::getQuads() const {
    return impl_->quads;
};

std::vector<ModelPtr>& RenderComponent::getModels() const {
    return impl_->models;
};

}  // namespace Presto
