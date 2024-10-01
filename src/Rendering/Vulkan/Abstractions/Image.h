#pragma once

#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/Abstractions/Abstraction.h"

namespace Presto {
    class Swapchain;

    class Image : public Abstraction<VkImage> {
       public:
        explicit Image(Swapchain&);
        ~Image();

       private:
        Swapchain* _swapchain;

        VkMemoryPropertyFlags _memoryFlags;
        VkDeviceMemory _memory;
    };
}  // namespace Presto
