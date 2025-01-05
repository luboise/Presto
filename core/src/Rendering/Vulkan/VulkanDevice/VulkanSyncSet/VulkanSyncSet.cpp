#include "VulkanSyncSet.h"
#include <vulkan/vulkan_core.h>

namespace PrestoCore {

    VulkanSyncSet::VulkanSyncSet(const VkDevice device) : _device(device) {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        // Fence begins signalled to not block GPU
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr,
                              &this->image_is_available) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to create Image Is Available semaphore.");
        }
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr,
                              &this->render_is_finished) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to create Render Finished semaphore.");
        }
        if (vkCreateFence(device, &fenceInfo, nullptr,
                          &this->frame_is_in_flight) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create fence.");
        }
    };

    // TODO: Fix the destructor
    VulkanSyncSet::~VulkanSyncSet() {
        vkDestroySemaphore(_device, this->render_is_finished, nullptr);
        vkDestroySemaphore(_device, this->image_is_available, nullptr);
        vkDestroyFence(_device, this->frame_is_in_flight, nullptr);
    };

}  // namespace PrestoCore
