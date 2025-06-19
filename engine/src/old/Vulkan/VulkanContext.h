#pragma once

#include <vulkan/vulkan_core.h>

namespace Presto {

struct VulkanContext {
    VkInstance instance;
    VkDevice device;
    VkPhysicalDevice physical_device;
};

}  // namespace Presto
