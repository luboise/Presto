#pragma once

#include <vulkan/vulkan_core.h>
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {

class Pipeline;
class VulkanBuffer;

struct VulkanDrawInfo : public DrawInfo {
    VulkanBuffer* vertex_buffer = nullptr;
    VulkanBuffer* index_buffer = nullptr;

    Pipeline* pipeline = nullptr;
};

struct DescriptorFrameSet {
    std::vector<VkDescriptorSet> sets;
    std::vector<std::unique_ptr<VulkanBuffer>> uniform_buffers;
};

using SetLayoutList = std::vector<VkDescriptorSetLayout>;
}  // namespace Presto
