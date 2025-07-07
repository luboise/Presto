module presto.objects.components.render;
import std;

import presto.core.logging;

namespace Pr {

struct RenderComponent::Impl {
    std::vector<QuadSubcomponent> quads;
    std::vector<Pr::Ptr<Pr::ModelAsset>> models;
};

RenderComponent::RenderComponent() { impl_ = std::make_unique<Impl>(); };
RenderComponent::~RenderComponent() = default;

void RenderComponent::onEnterScene() {
    for (Pr::Ptr<Pr::ModelAsset>& model : impl_->models) {
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

RenderComponent& RenderComponent::addModel(
    const Pr::Ptr<Pr::ModelAsset>& model) {
    if (model == nullptr) {
        Pr::Log(
            ERROR,
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

std::vector<Pr::Ptr<Pr::ModelAsset>>& RenderComponent::getModels() const {
    return impl_->models;
};

}  // namespace Pr
