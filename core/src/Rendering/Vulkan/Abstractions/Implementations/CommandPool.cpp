#include "../CommandPool.h"
#include <vulkan/vulkan_core.h>
#include <map>

#include "PrestoCore/Core.h"
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"

namespace PrestoCore {
    CommandPool::CommandPool(const VulkanDevice& device) : _device(device) {
        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        // Allows command buffers to be reset individually
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex =
            device.getQueueFamilyIndices().graphicsFamily.value();

        if (vkCreateCommandPool(_device.handle(), &createInfo, nullptr,
                                &_handle) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create command pool.");
        }
    }

    CommandBuffer* const CommandPool::createCommandBuffer() {
        auto* buffer = new CommandBuffer(*this);
        this->_commandBuffers[buffer] = {};

        return buffer;
    };

    void CommandPool::destroyCommandBuffer(CommandBuffer* const to_remove) {
        auto found = _commandBuffers.find(to_remove);

        PR_CORE_ASSERT(found != _commandBuffers.end(),
                       "An invalid command buffer can't be destroyed.");

        if (found != _commandBuffers.end()) {
            delete found->first;
            _commandBuffers.erase(found);
        }
    };

    CommandPool::~CommandPool() {
        vkResetCommandPool(_device.handle(), _handle, 0);
        vkDestroyCommandPool(_device.handle(), _handle, nullptr);
    }
}  // namespace PrestoCore
