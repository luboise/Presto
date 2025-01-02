#pragma once

#include "PrestoCore/Rendering/Renderer.h"

#include "Abstractions/Buffer.h"
#include "Abstractions/Swapchain.h"

#include "Managers/DrawManager/DrawManager.h"
#include "Managers/PipelineManager/PipelineManager.h"

// KEEP THESE SEPARATE
#include "Managers/BufferManager/BufferManager.h"
#include "Managers/DescriptorManager/DescriptorManager.h"

#include <GLFW/glfw3.h>

namespace Presto {
    class GLFWAppWindow;
    /*
       The renderer.
       */

    class PRESTO_API VulkanRenderer final : public Renderer {
       public:
        explicit VulkanRenderer(GLFWAppWindow* window);
        ~VulkanRenderer() override;

        void addToRenderPool(draw_info_key) override;
        void render(draw_info_key) override;
        void nextFrame() override;

       private:
        VulkanDevice* _device = nullptr;

        DrawManager* _drawManager = nullptr;
        PipelineManager* _pipelineManager = nullptr;
        BufferManager* _bufferManager = nullptr;
        DescriptorManager* _descriptorManager = nullptr;

        void initialiseSystems();
        void Shutdown();

        DrawManager::VulkanDrawContext _drawContext{};

        void onFrameBufferResized() override {
            this->_drawManager->queueReload();
        }

        // Background members
        std::vector<Pipeline*> _graphicsPipelines;

        VkSurfaceKHR _surface{};

        VkInstance _instance = VK_NULL_HANDLE;

        // Available graphics cards
        std::vector<VkPhysicalDevice> _physicalDevices;

        // The current physical device in use by the renderer
        // (assumed to always be only 1)
        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;

        VkDebugUtilsMessengerEXT _debugMessenger{};

        Swapchain* _swapchain = nullptr;

        static bool isDeviceSuitable(const VkPhysicalDevice&,
                                     const VkSurfaceKHR&);

        [[nodiscard]] VkSurfaceKHR createSurface() const;

        static std::vector<VkPhysicalDevice> getPhysicalDevices(
            const VkInstance&, const VkSurfaceKHR&);

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags,
                          VkMemoryPropertyFlags propFlags, VkBuffer& buffer,
                          VkDeviceMemory& bufferMemory);
    };
}  // namespace Presto
