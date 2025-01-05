#pragma once

#include "DescriptorGroup/DescriptorGroup.h"
#include "Rendering/Vulkan/Abstractions/DescriptorPool.h"
#include "Rendering/Vulkan/Abstractions/DescriptorSetLayout.h"
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"

namespace Presto {
    class BufferManager;

    class DescriptorManager {
       public:
        explicit DescriptorManager(VulkanDevice& device,
                                   BufferManager& buffer_manager)
            : _device(&device),
              _pool(_device->createDescriptorPool()),
              _bufferManager(&buffer_manager) {};

        DescriptorSetLayout* createDescriptorSetLayout(const BindingList&);

        DescriptorSetLayout* createDescriptorSetLayout(const Binding binding) {
            BindingList list = {binding};
            return createDescriptorSetLayout(list);
        };

        DescriptorGroup* createDescriptorGroup(
            const std::vector<DescriptorSetLayout*>&, uint32_t,
            BufferManager& buffer_manager);

       private:
        VulkanDevice* _device = nullptr;

        DescriptorPool* _pool = nullptr;
        BufferManager* _bufferManager = nullptr;

        std::vector<DescriptorSetLayout*> _layouts;
        std::vector<DescriptorGroup*> _groups;
    };
}  // namespace Presto
