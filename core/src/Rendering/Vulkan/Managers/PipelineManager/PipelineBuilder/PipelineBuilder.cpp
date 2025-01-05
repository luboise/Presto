#include "PipelineBuilder.h"

#include <Rendering/Vulkan/Abstractions/Pipeline.h>

#include "Rendering/Vulkan/Managers/DescriptorManager/DescriptorGroup/DescriptorGroup.h"

#include <vulkan/vulkan_core.h>
#include <algorithm>
#include "PrestoCore/Core.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanTypes.h"

namespace PrestoCore {

    Pipeline* PipelineBuilder::createPipeline() {
        /**
                for (const auto& layout :
           _pipelineLayout->getDescriptorSetLayouts()) {
                    this->_descSetLayouts.push_back(layout);
                }
                        **/

        PR_CORE_ASSERT(_renderContext != nullptr,
                       "Unable to create pipeline without a render context.");

        // Vertex input info (VBO/VBA)
        _vertexInput.vertexBindingDescriptionCount =
            static_cast<uint32_t>(_vertexBindingDescs.size());
        _vertexInput.pVertexBindingDescriptions = _vertexBindingDescs.data();

        _vertexInput.vertexAttributeDescriptionCount =
            static_cast<uint32_t>(_vertexAttributeDescs.size());
        _vertexInput.pVertexAttributeDescriptions =
            _vertexAttributeDescs.data();

        // Dynamic stages (can define later at render time)

        std::vector<VkDynamicState> dynamicStages = {};

        if (_useDynamicViewport) {
            dynamicStages.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        }
        if (_useDynamicScissor) {
            dynamicStages.push_back(VK_DYNAMIC_STATE_SCISSOR);
        }

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount =
            static_cast<uint32_t>(dynamicStages.size());
        dynamicState.pDynamicStates = dynamicStages.data();

        // Viewport
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        // TODO: Put the colour attachments into a vector
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        _colorBlending.attachmentCount = 1;
        _colorBlending.pAttachments = &colorBlendAttachment;

        VkGraphicsPipelineCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        auto transformed =
            std::vector<VkPipelineShaderStageCreateInfo>(_shaders.size());
        std::ranges::transform(
            _shaders, transformed.begin(),
            [](const auto& shader) { return shader->getPipelineCreateInfo(); });

        // Shader stages
        createInfo.stageCount = transformed.size();
        createInfo.pStages = transformed.data();

        createInfo.pVertexInputState = &_vertexInput;
        createInfo.pInputAssemblyState = &_inputAssembly;
        createInfo.pViewportState = &viewportState;
        createInfo.pRasterizationState = &_rasterizer;
        createInfo.pMultisampleState = &_multisampling;
        createInfo.pDepthStencilState = &_depthStencil;
        createInfo.pColorBlendState = &_colorBlending;
        createInfo.pDynamicState = &dynamicState;
        // createInfo.pTessellationState = nullptr;

        PR_CORE_ASSERT(_renderContext != nullptr,
                       "The render context must be set in order to create a "
                       "pipeline using the pipeline builder.");

        createInfo.renderPass = _renderContext->getRenderPass().handle();

        createInfo.subpass = 0;

        // Use when deriving from another pipeline
        createInfo.basePipelineHandle = VK_NULL_HANDLE;
        createInfo.basePipelineIndex = -1;

        auto* new_pipeline =
            new Pipeline(_device, _descriptorGroup, createInfo);

        this->_pipelineList->push_back(new_pipeline);

        return new_pipeline;
    };

    void PipelineBuilder::reset() {
        // Input assembly (first stage)
        _inputAssembly.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        _inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        _inputAssembly.primitiveRestartEnable = VK_FALSE;

        _vertexInput.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        _useDynamicScissor = true;
        _useDynamicViewport = true;

        _rasterizer.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        // Wireframe rendering
        _rasterizer.depthClampEnable = VK_FALSE;
        // Discard geometry past rasteriser
        _rasterizer.rasterizerDiscardEnable = VK_FALSE;
        // Fill polygon area with fragments
        _rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        // Line thickness in fragments
        _rasterizer.lineWidth = 1.0F;

        _rasterizer.cullMode = VK_CULL_MODE_NONE;
        _rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

        // Depth bias settings
        _rasterizer.depthBiasEnable = VK_FALSE;
        _rasterizer.depthBiasConstantFactor = 0.0F;  // Optional
        _rasterizer.depthBiasClamp = 0.0F;           // Optional
        _rasterizer.depthBiasSlopeFactor = 0.0F;     // Optional

        // Initial Depth Stencil
        _depthStencil.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        _depthStencil.depthTestEnable = VK_TRUE;
        _depthStencil.depthWriteEnable = VK_TRUE;
        _depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        _depthStencil.depthBoundsTestEnable = VK_FALSE;
        _depthStencil.stencilTestEnable = VK_FALSE;
        _depthStencil.front = {};
        _depthStencil.back = {};

        // Multisampling (currently disabled)
        _multisampling.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        _multisampling.sampleShadingEnable = VK_FALSE;
        _multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        _multisampling.minSampleShading = 1.0F;           // Optional
        _multisampling.pSampleMask = nullptr;             // Optional
        _multisampling.alphaToCoverageEnable = VK_FALSE;  // Optional
        _multisampling.alphaToOneEnable = VK_FALSE;       // Optional

        // Color blending options

        _colorBlending.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        _colorBlending.logicOpEnable = VK_FALSE;

        // POTENTIAL COLOUR BLEND CASE
        // colorBlendAttachment.blendEnable = VK_TRUE;
        // colorBlendAttachment.srcColorBlendFactor =
        // VK_BLEND_FACTOR_SRC_ALPHA;
        // colorBlendAttachment.dstColorBlendFactor =
        //     VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        // colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        // colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        // colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        // colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    }

    PipelineBuilder::PipelineBuilder(VulkanDevice& device,
                                     DescriptorGroup& group,
                                     std::vector<Pipeline*>& pipeline_list)
        : _device(device),
          _descriptorGroup(&group),
          _pipelineList(&pipeline_list) {
        this->reset();
    }
}  // namespace PrestoCore
