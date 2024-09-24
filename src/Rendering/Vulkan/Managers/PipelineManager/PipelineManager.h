#pragma once

#include <vulkan/vulkan_core.h>
#include "Rendering/Vulkan/Managers/DrawManager/RenderContext/RenderContext.h"
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"

#include "PipelineBuilder/PipelineBuilder.h"

namespace Presto {
    class PipelineManager {
       public:
        explicit PipelineManager(VulkanDevice*);

        PipelineBuilder getBuilder(DescriptorGroup& descriptor_group) {
            return PipelineBuilder(*_device, descriptor_group, _pipelines);
        };

        PipelineLayout* createPipelineLayout(
            const std::vector<VkDescriptorSetLayout>&);

       private:
        VulkanDevice* _device;

        std::vector<Pipeline*> _pipelines;
        std::vector<PipelineLayout*> _pipelineLayouts;

        //        void initialiseDefaultPipeline(VkRenderPass);
    };
}  // namespace Presto
