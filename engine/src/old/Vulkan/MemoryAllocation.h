#pragma once

#include <vulkan/vulkan_core.h>

namespace Presto {
struct VulkanContext;

class MemoryAllocation {
   public:
    MemoryAllocation(const VulkanContext&, VkMemoryRequirements requirements,
                     VkMemoryPropertyFlags flags);
    ~MemoryAllocation() = default;

    void bindTo(VkBuffer buffer);

    void unmap();

    void withMapping(auto lambda) {
        vkMapMemory(logicalDevice_, allocation_, /*offset*/ 0, size_, 0,
                    &mappedPtr_);

        if (mappedPtr_ == nullptr) {
            PR_ERROR(
                "Unable to map memory. Unable to proceed with mapping memory "
                "lambda function.");
            return;
        }

        std::span<std::byte> span{reinterpret_cast<std::byte*>(mappedPtr_),
                                  size_};
        lambda(span);

        this->unmap();
    }

   private:
    VkBuffer boundBuffer_{};
    Presto::size_t size_{};

    VkDeviceMemory allocation_{};
    void* mappedPtr_{};

    VkDevice logicalDevice_{};
};
}  // namespace Presto
