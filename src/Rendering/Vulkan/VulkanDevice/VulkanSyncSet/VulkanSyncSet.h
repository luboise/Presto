#pragma once

#include <vulkan/vulkan_core.h>

namespace Presto {
    struct VulkanSyncSet {
       public:
        VkSemaphore image_is_available = nullptr;
        VkSemaphore render_is_finished = nullptr;
        VkFence frame_is_in_flight = nullptr;

        explicit VulkanSyncSet(VkDevice);
        ~VulkanSyncSet();

       private:
        VkDevice _device;
    };
}  // namespace Presto
