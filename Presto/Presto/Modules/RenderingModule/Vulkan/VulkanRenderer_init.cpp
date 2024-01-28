#include "VulkanRenderer.h"

namespace Presto {
    void VulkanRenderer::createSurface() {
        if (glfwCreateWindowSurface(_instance, this->_glfwWindow, nullptr,
                                    &_surface) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create window surface.");
        }
    }
    void VulkanRenderer::createSwapChain() {
        SwapChainSupportDetails swapChainSupport =
            querySwapChainSupport(this->_physicalDevice);

        VkSurfaceFormatKHR surfaceFormat =
            chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode =
            chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D swapExtent = chooseSwapExtent(swapChainSupport.capabilities);

        // Want an extra image that we can render to while the first x are
        // performing operations
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0)
            if (imageCount > swapChainSupport.capabilities.maxImageCount) {
                imageCount = swapChainSupport.capabilities.maxImageCount;
            }

        VkSwapchainCreateInfoKHR createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = this->_surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;

        createInfo.imageExtent = swapExtent;

        // Higher for stereoscopic 3D
        createInfo.imageArrayLayers = 1;

        // Render it straight (colour attachment). Could change this later to do
        // post processing
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(this->_physicalDevice);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                         indices.presentFamily.value()};

        // Draw on graphics queue, then submit to presentation queue

        // If different queues, use concurrent mode
        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }  // Otherwise use exclusive
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;      // Optional
            createInfo.pQueueFamilyIndices = nullptr;  // Optional
        }

        // No rotation transformations
        createInfo.preTransform =
            swapChainSupport.capabilities.currentTransform;

        // Ignore alpha channel between windows
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;

        // Ignore pixels that are obscured (eg. behind other windows)
        createInfo.clipped = VK_TRUE;

        // No old swapchain exists
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(this->_logicalDevice, &createInfo, nullptr,
                                 &(this->_swapchain)) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create Vulkan swapchain.");
        }

        vkGetSwapchainImagesKHR(this->_logicalDevice, this->_swapchain,
                                &imageCount, nullptr);
        _swapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(this->_logicalDevice, this->_swapchain,
                                &imageCount, _swapchainImages.data());

        this->_swapchainImageFormat = surfaceFormat.format;
        this->_swapchainExtent = swapExtent;
    }
    void VulkanRenderer::createImageViews() {
        _swapchainImageViews.resize(_swapchainImages.size());

        for (size_t i = 0; i < _swapchainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};

            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = _swapchainImages[i];

            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = _swapchainImageFormat;

            // DONT swap any colour channels
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            // Using image for colour, and no mipmapping
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(_logicalDevice, &createInfo, nullptr,
                                  &(_swapchainImageViews[i])) != VK_SUCCESS) {
                throw std::runtime_error("Unable to create image views.");
            }
        };
    }

    void VulkanRenderer::createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = _swapchainImageFormat;

        // 1 Sample, no multisampling for color attachment
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

        // Clear to black before drawing
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        // Keep value in memory after computing
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        // Not using stencil buffer
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // Finalise image for presenting after render pass
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // This 0 is LOCATION 0
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        // Use the implicit (previous) subpass as a dependency
        VkSubpassDependency subpassDependency{};
        subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        // Index 0 is the only defined subpass
        subpassDependency.dstSubpass = 0;

        // THIS SUBPASS waits on color attachment stage before rendering to
        // framebuffer
        subpassDependency.srcStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.srcAccessMask = 0;

        // OTHER (future) SUBPASSES wait on color attachment before
        // transitioning to next subpass
        subpassDependency.dstStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        // Create the render pass info object
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;

        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        // Defines subpass transition behaviour
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &subpassDependency;

        if (vkCreateRenderPass(_logicalDevice, &renderPassInfo, nullptr,
                               &(this->_renderPass)) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create render pass.");
        }
    }

    void VulkanRenderer::createGraphicsPipeline() {
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};

        VkShaderModule vertShaderModule =
            createShaderModule("Shaders/Core/basic_triangle_vert.spv");
        vertShaderStageInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";
        // Entrypoint of shader ^

        VkShaderModule fragShaderModule =
            createShaderModule("Shaders/Core/basic_triangle_frag.spv");
        fragShaderStageInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";
        // Entrypoint of shader ^

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                          fragShaderStageInfo};

        auto bindingDescription = VulkanVertex::getBindingDescription();
        auto attributeDescriptions = VulkanVertex::getAttributeDescriptions();

        // Vertex input info (VBO/VBA)
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

        vertexInputInfo.vertexAttributeDescriptionCount =
            static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions =
            attributeDescriptions.data();

        // Input assembly (first stage)

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        // Dynamic stages (can define later at render time)
        std::vector<VkDynamicState> dynamicStages = {VK_DYNAMIC_STATE_VIEWPORT,
                                                     VK_DYNAMIC_STATE_SCISSOR};
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

        // Rasteriser
        VkPipelineRasterizationStateCreateInfo rasterizer =
            makeRasterizerInfo();

        // Multisampling (currently disabled)
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;           // Optional
        multisampling.pSampleMask = nullptr;             // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE;  // Optional
        multisampling.alphaToOneEnable = VK_FALSE;       // Optional

        // Color blending options
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

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

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pNext = NULL;

        // Number of uniform variables
        pipelineLayoutInfo.setLayoutCount = 1;                   // Optional
        pipelineLayoutInfo.pSetLayouts = &_descriptorSetLayout;  // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0;           // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr;        // Optional

        if (vkCreatePipelineLayout(_logicalDevice, &pipelineLayoutInfo, nullptr,
                                   &_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error(
                "Unable to create graphics pipeline layout.");
        }

        VkGraphicsPipelineCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.stageCount = 2;
        createInfo.pStages = shaderStages;

        createInfo.pVertexInputState = &vertexInputInfo;
        createInfo.pInputAssemblyState = &inputAssembly;
        createInfo.pViewportState = &viewportState;
        createInfo.pRasterizationState = &rasterizer;
        createInfo.pMultisampleState = &multisampling;
        createInfo.pDepthStencilState = nullptr;
        createInfo.pColorBlendState = &colorBlending;
        createInfo.pDynamicState = &dynamicState;
        // createInfo.pTessellationState = nullptr;

        createInfo.layout = this->_pipelineLayout;

        createInfo.renderPass = this->_renderPass;
        createInfo.subpass = 0;

        // Use when deriving from another pipeline
        createInfo.basePipelineHandle = VK_NULL_HANDLE;
        createInfo.basePipelineIndex = -1;

        auto res =
            vkCreateGraphicsPipelines(_logicalDevice, VK_NULL_HANDLE, 1,
                                      &createInfo, nullptr, &_graphicsPipeline);

        vkDestroyShaderModule(_logicalDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(_logicalDevice, vertShaderModule, nullptr);

        if (res != VK_SUCCESS) {
            throw std::runtime_error("Unable to create graphics pipeline.");
        }
    };

    void VulkanRenderer::createFrameBuffers() {
        // Get framebuffers from image view (?)
        this->_swapchainFramebuffers.resize(_swapchainImageViews.size());

        for (size_t i = 0; i < _swapchainImageViews.size(); i++) {
            VkImageView attachments[] = {_swapchainImageViews[i]};

            VkFramebufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass = this->_renderPass;
            createInfo.attachmentCount = 1;
            createInfo.pAttachments = attachments;
            createInfo.width = _swapchainExtent.width;
            createInfo.height = _swapchainExtent.height;
            createInfo.layers = 1;

            if (vkCreateFramebuffer(_logicalDevice, &createInfo, nullptr,
                                    &_swapchainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Unable to create framebuffer.");
            }
        }
    }

    void VulkanRenderer::createCommandPool() {
        QueueFamilyIndices familyIndices = findQueueFamilies(_physicalDevice);

        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        // Allows command buffers to be reset individually
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex = familyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(_logicalDevice, &createInfo, nullptr,
                                &_commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create command pool.");
        }
    }

    void VulkanRenderer::createBuffers() {
        VkDeviceSize bufferSize;

        // Vertex buffers
        bufferSize = sizeof(vertices[0]) * vertices.size();
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     _stagingBuffer, _stagingBufferMemory);

        createBuffer(bufferSize,
                     VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer,
                     _vertexBufferMemory);

        // Index buffer
        bufferSize = sizeof(indices[0]) * indices.size();
        createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer,
            _indexBufferMemory);

        // Uniform buffers
        bufferSize = sizeof(ShaderMatrices);

        _uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        _uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
        _uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         _uniformBuffers[i], _uniformBuffersMemory[i]);

            vkMapMemory(_logicalDevice, _uniformBuffersMemory[i], 0, bufferSize,
                        0, &_uniformBuffersMapped[i]);
        }
    }

    void VulkanRenderer::createDescriptorPool() {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        createInfo.poolSizeCount = 1;
        createInfo.pPoolSizes = &poolSize;

        if (vkCreateDescriptorPool(_logicalDevice, &createInfo, nullptr,
                                   &_descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create descriptor pool.");
        };
    }

    void VulkanRenderer::createDescriptorSets() {
        std::vector<VkDescriptorSetLayout> setLayouts(MAX_FRAMES_IN_FLIGHT,
                                                      _descriptorSetLayout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = _descriptorPool;
        allocInfo.descriptorSetCount =
            static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = setLayouts.data();

        _descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(_logicalDevice, &allocInfo,
                                     _descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("Unable to allocate descriptor sets.");
        };

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = _uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(ShaderMatrices);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = _descriptorSets[i];
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            // Unused
            descriptorWrite.dstBinding = NULL;
            descriptorWrite.dstArrayElement = NULL;

            descriptorWrite.pImageInfo = nullptr;
            descriptorWrite.pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(_logicalDevice, 1, &descriptorWrite, 0,
                                   nullptr);
        }
    }

    void VulkanRenderer::initialiseBuffers() {
        VkDeviceSize bufferSize;
        void* data;

        // Copy vertices into staging buffer
        bufferSize = sizeof(vertices[0]) * vertices.size();
        vkMapMemory(_logicalDevice, _stagingBufferMemory, 0, bufferSize, 0,
                    &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(_logicalDevice, _stagingBufferMemory);

        copyBuffer(_stagingBuffer, _vertexBuffer, bufferSize);

        // Copy indices into index buffer
        bufferSize = sizeof(indices[0]) * indices.size();
        vkMapMemory(_logicalDevice, _stagingBufferMemory, 0, bufferSize, 0,
                    &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(_logicalDevice, _stagingBufferMemory);

        copyBuffer(_stagingBuffer, _indexBuffer, bufferSize);
    }

    void VulkanRenderer::createCommandBuffers() {
        _commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = _commandPool;
        // Primary commandbuffer (can submit directly)
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

        if (vkAllocateCommandBuffers(_logicalDevice, &allocInfo,
                                     _commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create command buffers.");
        }
    }

    void VulkanRenderer::createSyncObjects() {
        _imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        _renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

        // Fence begins signalled to not block GPU
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(_logicalDevice, &semaphoreInfo, nullptr,
                                  &_imageAvailableSemaphores[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("Failed to create semaphore.");
            }
            if (vkCreateSemaphore(_logicalDevice, &semaphoreInfo, nullptr,
                                  &_renderFinishedSemaphores[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("Failed to create semaphore.");
            }
            if (vkCreateFence(_logicalDevice, &fenceInfo, nullptr,
                              &_inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create semaphore.");
            }
        }
    }

    VkPipelineRasterizationStateCreateInfo
    VulkanRenderer::makeRasterizerInfo() {
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

        // Wireframe rendering
        rasterizer.depthClampEnable = VK_FALSE;
        // Discard geometry past rasteriser
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        // Fill polygon area with fragments
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        // Line thickness in fragments
        rasterizer.lineWidth = 1.0f;

        rasterizer.cullMode = VK_CULL_MODE_NONE;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

        // Depth bias settings
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;  // Optional
        rasterizer.depthBiasClamp = 0.0f;           // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f;     // Optional

        return rasterizer;
    }
}  // namespace Presto