#pragma once

#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/Abstractions/Abstraction.h"

namespace PrestoCore {
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

        explicit operator VkDescriptorSetLayout() { return _handle; };

       private:
        const VulkanDevice& _device;
    };
}  // namespace PrestoCore
