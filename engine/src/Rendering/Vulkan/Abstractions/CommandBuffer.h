#pragma once

#include <vulkan/vulkan_core.h>

#include "Abstraction.h"

#include "Rendering/Vulkan/Abstractions/Pipeline.h"

#include "Rendering/Vulkan/VulkanUtils/VulkanTypes.h"

namespace Presto {
class CommandPool;
class RenderPass;

class CommandBuffer : public Abstraction<VkCommandBuffer> {
    friend class CommandPool;

   public:
    void reset() { vkResetCommandBuffer(handle_, 0); }

    void startRecording(uint flags);
    void startRecording() { this->startRecording(0); };
    void stopRecording();

    void beginPass(const RenderPass&, VkFramebuffer);

    void bind(Pipeline&);
    void bind(const DescriptorFrameSet&);
    void bind(const VulkanDrawInfo&);
    void bind(const std::vector<VulkanBuffer*>&, std::vector<unsigned long>);

    void bind(VulkanBuffer& buffer, unsigned long offset) {
        std::vector<VulkanBuffer*> buffers = {&buffer};
        this->bind(buffers, {offset});
    };

   private:
    explicit CommandBuffer(CommandPool&);
    ~CommandBuffer();

    CommandPool& _pool;

    Pipeline* _currentBoundPipeline = nullptr;

    bool _recording = false;
};
}  // namespace Presto
