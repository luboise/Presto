#pragma once

#include <vulkan/vulkan_core.h>

#include "Abstraction.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanTypes.h"

using std::vector;

namespace PrestoCore {
    class VulkanDevice;

    class PipelineLayout : public Abstraction<VkPipelineLayout> {
       public:
        explicit PipelineLayout(VulkanDevice*, const SetLayoutList&);

        ~PipelineLayout();

        [[nodiscard]] vector<VkDescriptorSetLayout> getDescriptorSetLayouts()
            const {
            return this->_descriptorSetLayouts;
        }

       private:
        VulkanDevice* _device = nullptr;
        SetLayoutList _descriptorSetLayouts;
    };
}  // namespace PrestoCore
