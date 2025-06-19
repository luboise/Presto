module presto.objects.components.canvas;

// export module presto.objects.components:canvas;

export namespace Presto {

CanvasGroup& CanvasComponent::addGroup(CanvasGroup group) {
    CanvasGroup& new_group{groups_.emplace_back(std::move(group))};
    return new_group;
};

CanvasGroup& CanvasComponent::newGroup() {
    CanvasGroup& new_group{groups_.emplace_back(CanvasGroup{})};
    return new_group;
};

CanvasComponent::CanvasComponent() = default;

VisualExtents CanvasComponent::size() const { return size_; };

void CanvasComponent::setSize(VisualExtents extents) { size_ = extents; }

CanvasGroup* CanvasComponent::group(Presto::size_t index) {
    if (index >= groups_.size()) {
        return nullptr;
    }

    return &groups_[index];
};

}  // namespace Presto
