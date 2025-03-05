
#include "../DescriptorSetLayout.h"

#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"

namespace Presto {
DescriptorSetLayout::DescriptorSetLayout(const VulkanDevice& device,
                                         const BindingList& bindings)
    : _device(device) {
    // Create descriptor set with binding
    VkDescriptorSetLayoutCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.bindingCount = bindings.size();
    createInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(_device.handle(), &createInfo, nullptr,
                                    &handle_) != VK_SUCCESS) {
        throw std::runtime_error("Unable to create descriptor set layout.");
    };
}

DescriptorSetLayout::~DescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(_device.handle(), handle_, nullptr);
}
}  // namespace Presto
