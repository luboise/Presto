#include "../Image.h"

#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/Abstractions/Swapchain.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanUtils.h"

namespace Presto {
    Image::Image(Swapchain& swapchain, VkFormat format,
                 VkImageTiling image_tiling, VkImageUsageFlags usage_flags,
                 VkMemoryPropertyFlags memory_flags)
        : Abstraction(), _swapchain(&swapchain), _memoryFlags(memory_flags) {
        auto extent = _swapchain->getExtent();

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = extent.width;
        imageInfo.extent.height = extent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = image_tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage_flags;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        auto* device = _swapchain->getDevice();

        if (vkCreateImage(device->handle(), &imageInfo, nullptr, &_handle) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        _memory = device->allocate();
    };
}  // namespace Presto
