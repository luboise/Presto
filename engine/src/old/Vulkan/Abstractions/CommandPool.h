#pragma once

#include <vulkan/vulkan_core.h>
#include <map>
#include "Abstraction.h"
#include "CommandBuffer.h"

namespace Presto {
class VulkanDevice;

class CommandPool : public Abstraction<VkCommandPool> {
    struct CommandBufferDetails {};

   public:
    CommandPool(const VulkanDevice&);
    ~CommandPool();

    CommandBuffer* createCommandBuffer();
    void destroyCommandBuffer(CommandBuffer* const);

    inline const VulkanDevice& getLogicalDevice() { return this->_device; }

   private:
    const VulkanDevice& _device;

    std::map<CommandBuffer*, CommandBufferDetails> _commandBuffers;
};
}  // namespace Presto
