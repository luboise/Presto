#include "../DescriptorPool.h"
#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"

namespace Presto {

DescriptorPool::DescriptorPool(const VulkanDevice& device)
    : Abstraction(), _device(device) {
    constexpr auto POOL_SIZE_COUNT = 1;
    constexpr auto UNIFORM_BUFFER_COUNT = 50;

    VkDescriptorPoolSize poolSizes[POOL_SIZE_COUNT]{};

    // Allocates "count_of_each_descriptor_type" uniform buffers in the pool
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    // TODO: Make this value modular
    poolSizes[0].descriptorCount = static_cast<uint32_t>(UNIFORM_BUFFER_COUNT);

    // Later on, this could allocate count_of_each_descriptor_type image
    // samplers as well, in the same pool
    /**
poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
poolSizes[1].descriptorCount =
count_of_each_descriptor_type;  // For combined image samplers
            **/

    // The pool create info records how many "pool size" structs there are
    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets = static_cast<uint32_t>(UNIFORM_BUFFER_COUNT);
    createInfo.poolSizeCount = POOL_SIZE_COUNT;
    createInfo.pPoolSizes = poolSizes;

    if (vkCreateDescriptorPool(_device.handle(), &createInfo, nullptr,
                               &handle_) != VK_SUCCESS) {
        throw std::runtime_error("Unable to create descriptor pool.");
    };
}

DescriptorPool::operator VkDescriptorPool() const { return this->handle_; };

DescriptorPool::~DescriptorPool() {
    auto* device_handle = _device.handle();
    vkResetDescriptorPool(device_handle, handle_, 0);
    vkDestroyDescriptorPool(device_handle, handle_, nullptr);
}

std::vector<VkDescriptorSet> DescriptorPool::allocate(
    const std::vector<VkDescriptorSetLayout>& layouts) {
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = handle_;

    const auto& device = _device;

    allocInfo.descriptorSetCount = layouts.size();
    allocInfo.pSetLayouts = layouts.data();

    auto allocated = std::vector<VkDescriptorSet>(layouts.size());

    if (vkAllocateDescriptorSets(device.handle(), &allocInfo,
                                 allocated.data()) != VK_SUCCESS) {
        throw std::runtime_error(
            fmt::format("Unable to allocate {} descriptor sets from "
                        "DescriptorPool {}.",
                        layouts.size(), fmt::ptr(this), fmt::ptr(handle_)));
    }

    return allocated;
}
}  // namespace Presto
