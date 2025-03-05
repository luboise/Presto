#pragma once

#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/Abstractions/Abstraction.h"

namespace Presto {
class VulkanDevice;

using Binding = VkDescriptorSetLayoutBinding;
using BindingList = std::vector<Binding>;

class DescriptorSetLayout : public Abstraction<VkDescriptorSetLayout> {
   public:
    DescriptorSetLayout(const VulkanDevice&, const BindingList&);

    // The layout is deallocated when the descriptor pool is deallocated,
    // but a destructor could be put here for individual destruction.

    // TODO: Put an individual destructor here
    ~DescriptorSetLayout();

    explicit operator VkDescriptorSetLayout() { return handle_; };

   private:
    const VulkanDevice& _device;
};
}  // namespace Presto
