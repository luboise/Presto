#pragma once

#include "PrestoCore/Rendering/RenderTypes.h"
#include "Rendering/Vulkan/Abstractions/Buffer.h"

namespace Presto {
    class Pipeline;

    struct VulkanDrawInfo : public DrawInfo {
        Buffer* vertex_buffer = nullptr;
        Buffer* index_buffer = nullptr;

        Pipeline* pipeline = nullptr;
    };

    struct DescriptorFrameSet {
        std::vector<VkDescriptorSet> sets;
        std::vector<std::unique_ptr<Buffer>> uniform_buffers;
    };

    using SetLayoutList = std::vector<VkDescriptorSetLayout>;
}  // namespace Presto
