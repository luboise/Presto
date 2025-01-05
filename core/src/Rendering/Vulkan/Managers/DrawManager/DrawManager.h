#pragma once

#include <vulkan/vulkan_core.h>

#include <Rendering/Vulkan/Abstractions/Buffer.h>
#include "Rendering/Vulkan/Abstractions/CommandPool.h"
#include "Rendering/Vulkan/Abstractions/Swapchain.h"

#include "Rendering/Vulkan/VulkanDevice/VulkanSyncSet/VulkanSyncSet.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanTypes.h"

#include "RenderContext/RenderContext.h"

namespace PrestoCore {
    class DescriptorPool;
    class Swapchain;
    class CommandBuffer;

    using RenderCtxKey = std::string;
    using UniformList = std::vector<Buffer*>*;

    class DrawManager {
       public:
        struct VulkanDrawContext {
            CommandBuffer* command_buffer;
            VulkanSyncSet* sync_set;
        };

        DrawManager(Swapchain&, uint32_t);

        [[nodiscard]] CommandPool* getCommandPool() const {
            return _commandPool;
        }

        PR_RESULT enableDrawing();
        void draw(const VulkanDrawInfo&);
        void disableDrawing();
        void submitCommands();

        // Adds a new layer and returns the number of the new layer
        RenderContext* addLayer(const RenderCtxKey& layer_name);

        void setActiveLayer(const RenderCtxKey& key) {
            this->_currentRenderContext = _renderContexts[key];
        }

        // Gets the layer at a specific index
        RenderContext* getLayer(const RenderCtxKey& key) {
            return this->_renderContexts[key];
        }

        void goToNextFrame();

        inline void queueReload() { this->_reloadSwapchainOnNextFrame = true; }

        [[nodiscard]] uint32_t getCurrentFrame() const {
            return this->_currentFrame;
        }

        /**
void AddToRenderPool(draw_info_key);
void draw(draw_info_key);
void nextFrame();
**/

        ~DrawManager();

       private:
        Swapchain& _swapchain;

        bool _startedDrawing = false;
        uint32_t _currentImageIndex = 0;

        bool _reloadSwapchainOnNextFrame = false;

        void destroyFramebuffers();

        void reloadSwapchain();

        static VkRenderPass createRenderPass(const Swapchain&);

        void createFramebuffers(const Swapchain& swapchain,
                                const RenderPass& pass);

        inline void advanceFrame() {
            this->_currentFrame++;
            this->_drawContext = getDrawContext();
        };

        [[nodiscard]] struct VulkanDrawContext getDrawContext();

        CommandPool* _commandPool;

        RenderContext* _currentRenderContext;

        std::map<RenderCtxKey, RenderContext*> _renderContexts;

        uint32_t _currentFrame = 0;
        struct VulkanDrawContext _drawContext;

        //        std::vector<VkDescriptorSetLayout> _descriptorSetLayouts;

        std::vector<VkFramebuffer> _framebuffers;
        std::vector<CommandBuffer*> _commandBuffers;

        std::vector<VulkanSyncSet*> _syncSets;

        // Global descriptor set and model descriptor set for each frame in
        // flight
    };
}  // namespace PrestoCore
