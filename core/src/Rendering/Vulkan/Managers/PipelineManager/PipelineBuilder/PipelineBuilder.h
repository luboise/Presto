#pragma once

#include "Rendering/Vulkan/Abstractions/Pipeline.h"

#include "Rendering/Vulkan/Abstractions/RenderPass.h"
#include "Rendering/Vulkan/Managers/DrawManager/RenderContext/RenderContext.h"
#include "Rendering/Vulkan/VulkanShader/VulkanShader.h"

#include <vulkan/vulkan_core.h>

namespace PrestoCore {
    class DescriptorGroup;
    class VulkanDevice;
    class Pipeline;
    class PipelineLayout;

    class PipelineBuilder {
       public:
        explicit PipelineBuilder(VulkanDevice&, DescriptorGroup&,
                                 std::vector<Pipeline*>&);

        Pipeline* createPipeline();

        void reset();

        PipelineBuilder& setRenderContext(RenderContext& ctx) {
            this->_renderContext = &ctx;
            return *this;
        }

        PipelineBuilder& setTopology(const VkPrimitiveTopology topology) {
            _inputAssembly.topology = topology;
            return *this;
        };

        PipelineBuilder& addShader(VulkanShader* new_shader) {
            _shaders.push_back(new_shader);
            return *this;
        }

        PipelineBuilder& addVertexBindingDescription(
            const VkVertexInputBindingDescription desc) {
            this->_vertexBindingDescs.push_back(desc);

            return *this;
        };

        PipelineBuilder& addVertexAttributeDescription(
            const VkVertexInputAttributeDescription desc) {
            this->_vertexAttributeDescs.push_back(desc);

            return *this;
        };

        PipelineBuilder& useDynamicViewport(const bool use) {
            this->_useDynamicViewport = use;

            return *this;
        };

        PipelineBuilder& useDynamicScissor(const bool use) {
            this->_useDynamicScissor = use;
            return *this;
        };

        /**
PipelineBuilder& addDescriptorSetLayout(
    const VkDescriptorSetLayout layout) {
    this->_descSetLayouts.push_back(layout);
    return *this;
}
        **/

       private:
        VulkanDevice& _device;
        DescriptorGroup* _descriptorGroup = nullptr;
        RenderContext* _renderContext = nullptr;

        std::vector<Pipeline*>* _pipelineList;

        std::vector<VulkanShader*> _shaders;

        VkPipelineInputAssemblyStateCreateInfo _inputAssembly{};
        VkPipelineVertexInputStateCreateInfo _vertexInput{};

        VkPipelineRasterizationStateCreateInfo _rasterizer{};
        VkPipelineDepthStencilStateCreateInfo _depthStencil{};
        VkPipelineMultisampleStateCreateInfo _multisampling{};
        VkPipelineColorBlendStateCreateInfo _colorBlending{};

        std::vector<VkVertexInputBindingDescription> _vertexBindingDescs;
        std::vector<VkVertexInputAttributeDescription> _vertexAttributeDescs;

        bool _useDynamicViewport = true;
        bool _useDynamicScissor = true;
    };
}  // namespace PrestoCore
