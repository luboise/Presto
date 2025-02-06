#include <vulkan/vulkan_core.h>

#include "PipelineManager.h"

namespace Presto {

PipelineManager::PipelineManager(VulkanDevice* device)
    : _device(device) {
          //        initialiseDefaultPipeline(defaultPipelineRenderPass);
      };

/*
PipelineLayout* PipelineManager::createPipelineLayout(
const std::vector<VkDescriptorSetLayout>& set_layouts) {
auto* pipeline_layout = new PipelineLayout(_device, set_layouts);

this->_pipelineLayouts.push_back(pipeline_layout);

return pipeline_layout;
}
*/

/*
    void PipelineManager::initialiseDefaultPipeline(VkRenderPass pass) {
        std::vector<VkDescriptorSetLayout> set_layouts = {
            _globalSetLayout->handle(), _localSetLayout->handle()};
        auto* layout = this->createPipelineLayout(set_layouts);

        auto builder = this->getBuilder(layout);

        this->_defaultPipeline = builder.createPipeline();
    };
        */

}  // namespace Presto
