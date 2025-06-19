#pragma once

#include <vulkan/vulkan_core.h>

#include "Abstraction.h"
#include "PipelineLayout.h"
#include "Rendering/Vulkan/Managers/DescriptorManager/DescriptorGroup/DescriptorGroup.h"

namespace Presto {
class VulkanDevice;

class Pipeline : public Abstraction<VkPipeline> {
    friend class PipelineBuilder;

   public:
    ~Pipeline();

    [[nodiscard]] PipelineLayout* layout() const { return _layout; }
    [[nodiscard]] DescriptorGroup& getDescriptors() const {
        return *_descriptorGroup;
    }

   private:
    Pipeline(VulkanDevice&, DescriptorGroup*, VkGraphicsPipelineCreateInfo&);

    VkDevice _logicalDevice = VK_NULL_HANDLE;
    PipelineLayout* _layout = VK_NULL_HANDLE;
    DescriptorGroup* _descriptorGroup;
};
}  // namespace Presto
