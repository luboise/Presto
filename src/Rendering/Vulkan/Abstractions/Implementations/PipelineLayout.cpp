
#include "../PipelineLayout.h"
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"

namespace Presto {

    PipelineLayout::PipelineLayout(
        VulkanDevice* device,
        const std::vector<VkDescriptorSetLayout>& set_layouts)
        : Abstraction(), _device(device) {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pNext = nullptr;

        // Uniform Data
        pipelineLayoutInfo.setLayoutCount = set_layouts.size();
        pipelineLayoutInfo.pSetLayouts = set_layouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(_device->handle(), &pipelineLayoutInfo,
                                   nullptr, &_handle) != VK_SUCCESS) {
            throw std::runtime_error(
                "Unable to create graphics pipeline layout.");
        }

        this->_descriptorSetLayouts = set_layouts;
    }

    PipelineLayout::~PipelineLayout() {
        vkDestroyPipelineLayout(_device->handle(), _handle, nullptr);
    }
}  // namespace Presto
