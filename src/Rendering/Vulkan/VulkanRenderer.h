#pragma once

#include "Presto/Rendering/Renderer.h"

#include "Abstractions/Buffer.h"
#include "Abstractions/Swapchain.h"

#include "Managers/DrawManager/DrawManager.h"
#include "Managers/PipelineManager/PipelineManager.h"

// KEEP THESE SEPARATE
#include "Managers/BufferManager/BufferManager.h"
#include "Managers/DescriptorManager/DescriptorManager.h"
#include "VkBootstrap.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

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
        void draw(draw_info_key) override;
        void nextFrame() override;

       private:
        vkb::Device _device;

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

        vkb::Instance _instance;

        // Available graphics cards

        vkb::PhysicalDevice _phys_device;

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
