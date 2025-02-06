#include "../Pipeline.h"
#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"

#include "Rendering/Vulkan/Managers/DescriptorManager/DescriptorGroup/DescriptorGroup.h"

// #include "Presto/Logging.h"

namespace Presto {

Pipeline::Pipeline(VulkanDevice& device, DescriptorGroup* descriptor_group,
                   VkGraphicsPipelineCreateInfo& info)
    : Abstraction(),
      _logicalDevice(device.handle()),
      _descriptorGroup(descriptor_group) {
    auto* pipelineLayout =
        new PipelineLayout(&device, descriptor_group->getLayouts());

    /*
if (info.layout != layout.handle()) {
PR_CORE_WARN(
    "The layout ({}) in the info received by the Pipeline "
    "constructor "
    "does not match the handle of the layout argument passed ({}). "
    "Overriding using the argument passed.",
    fmt::ptr(info.layout), fmt::ptr(&layout));
}
    */

    this->_layout = pipelineLayout;
    info.layout = pipelineLayout->handle();

    if (vkCreateGraphicsPipelines(_logicalDevice, VK_NULL_HANDLE, 1, &info,
                                  nullptr, &_handle) != VK_SUCCESS) {
        throw std::runtime_error("Unable to create graphics pipeline.");
    }
}

Pipeline::~Pipeline() {
    vkDestroyPipeline(_logicalDevice, _handle, nullptr);
    delete _layout;
};
}  // namespace Presto
