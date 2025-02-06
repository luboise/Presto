#include "DescriptorManager.h"

namespace Presto {

DescriptorSetLayout* DescriptorManager::createDescriptorSetLayout(
    const BindingList& bindings) {
    auto* layout = new DescriptorSetLayout(*_device, bindings);

    this->_layouts.push_back(layout);
    return layout;
}

DescriptorGroup* DescriptorManager::createDescriptorGroup(
    const std::vector<DescriptorSetLayout*>& layouts, uint32_t count,
    BufferManager& buffer_manager) {
    auto* group = new DescriptorGroup(*_pool, *_bufferManager, layouts, count);
    this->_groups.push_back(group);

    return group;
}
// Overload which creates a vector out of the single binding passed and
// sends that to the regular function

}  // namespace Presto
