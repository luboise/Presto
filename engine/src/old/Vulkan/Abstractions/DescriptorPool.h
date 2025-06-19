#pragma once

#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/Abstractions/Abstraction.h"
#include "Rendering/Vulkan/Abstractions/DescriptorSetLayout.h"

namespace Presto {
class VulkanDevice;
class DescriptorGroup;

class DescriptorPool : public Abstraction<VkDescriptorPool> {
   public:
    explicit DescriptorPool(const VulkanDevice&);
    ~DescriptorPool();

    explicit operator VkDescriptorPool() const;

    std::vector<VkDescriptorSet> allocate(
        const std::vector<VkDescriptorSetLayout>&);

    [[nodiscard]] const VulkanDevice& getDevice() const {
        return this->_device;
    }

   private:
    const VulkanDevice& _device;
    std::vector<VkDescriptorSet> _allocations;
};
}  // namespace Presto
