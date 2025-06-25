#pragma once

#include <vulkan/vulkan_core.h>

namespace Pr {

struct VulkanContext {
    VkInstance instance;
    VkDevice device;
    VkPhysicalDevice physical_device;
};

}  // namespace Pr
