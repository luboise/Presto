#include "MemoryAllocation.h"

#include "Rendering/Vulkan/VulkanContext.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanUtils.h"

namespace Presto {
MemoryAllocation::MemoryAllocation(const VulkanContext& ctx,
                                   VkMemoryRequirements requirements,
                                   VkMemoryPropertyFlags flags) {
    VkMemoryAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = requirements.size,
        .memoryTypeIndex = VulkanUtils::findMemoryType(
            ctx.physical_device, requirements.memoryTypeBits, flags)};

    if (vkAllocateMemory(ctx.device, &allocInfo, nullptr, &allocation_) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate vertex buffer memory.");
    }

    size_ = requirements.size;
};

void MemoryAllocation::bindTo(VkBuffer buffer) {
    // Bind allocate memory to buffer (at offset 0, otherwise must
    // be divisible by memRequirements.alignment)
    vkBindBufferMemory(logicalDevice_, buffer, allocation_, 0);
    boundBuffer_ = buffer;
}

void MemoryAllocation::unmap() {
    if (mappedPtr_ == nullptr) {
        return;
    }
    vkUnmapMemory(logicalDevice_, allocation_);
    mappedPtr_ = nullptr;
}
}  // namespace Presto
