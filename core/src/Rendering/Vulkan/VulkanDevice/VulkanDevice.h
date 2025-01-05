#pragma once

#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/VulkanDevice/VulkanSyncSet/VulkanSyncSet.h"

#include "Rendering/Vulkan/QueueFamilyIndices/QueueFamilyIndices.h"

#include <Rendering/Vulkan/Abstractions/Buffer.h>
#include "Rendering/Vulkan/Abstractions/Abstraction.h"
#include "Rendering/Vulkan/Abstractions/CommandPool.h"
#include "Rendering/Vulkan/Abstractions/DescriptorPool.h"
#include "Rendering/Vulkan/Abstractions/DescriptorSetLayout.h"

namespace PrestoCore {
    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanDevice : public Abstraction<VkDevice> {
       public:
        VulkanDevice(const VkPhysicalDevice&, const VkSurfaceKHR&);
        ~VulkanDevice();

        [[nodiscard]] const VkPhysicalDevice& getPhysicalDevice() const;

        [[nodiscard]] const QueueFamilyIndices& getQueueFamilyIndices() const;

        DescriptorPool* createDescriptorPool();
        Buffer* createBuffer(Buffer::BUFFER_TYPE, VkDeviceSize);
        CommandPool* createCommandPool();

        VulkanSyncSet* createSyncSet();

        [[nodiscard]] SwapchainSupportDetails getSwapchainSupportDetails()
            const;

        static SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice,
                                                             VkSurfaceKHR);

        void submitToGraphicsQueue(
            const std::vector<VkCommandBuffer>& buffers) const;

        void submitToGraphicsQueue(const VkCommandBuffer buffer) const {
            std::vector<VkCommandBuffer> vec = {buffer};
            submitToGraphicsQueue(vec);
        };

        [[nodiscard]] VkQueue getGraphicsQueue() const {
            return _graphicsQueue;
        }

        [[nodiscard]] VkQueue getPresentQueue() const { return _presentQueue; }

       private:
        QueueFamilyIndices _indices;

        VkPhysicalDevice _physicalDevice;
        VkSurfaceKHR _surface;

        void createLogicalDevice();

        void retrieveQueueHandles();
        VkQueue _graphicsQueue;
        VkQueue _presentQueue;

        std::vector<DescriptorSetLayout*> _descriptorSetLayouts;

        std::vector<DescriptorPool*> _descriptorPools;
        std::vector<Buffer*> _buffers;

        std::vector<CommandPool*> _commandPools;
        std::vector<VulkanSyncSet*> _syncSets;

        void createSwapchain();

        static const VkDescriptorSetLayout allocateDescriptorSetLayout(
            const VulkanDevice&);
    };
}  // namespace PrestoCore
