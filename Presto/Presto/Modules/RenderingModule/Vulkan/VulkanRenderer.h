#pragma once

#include "Presto/Modules/RenderingModule/_Renderer.h"
#include "VulkanVertex.h"

#include <vulkan/vulkan.h>

#include "Presto/Modules/ObjectsModule/Entity/Entity.h"
#include "Presto/Modules/RenderingModule/Types/DrawInfo.h"

// KEEP THESE SEPARATE
#include <GLFW/glfw3.h>

#include <optional>

namespace Presto {
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    const std::vector<const char*> extensionList = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    const int MAX_FRAMES_IN_FLIGHT = 2;

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return this->graphicsFamily.has_value() &&
                   presentFamily.has_value();
        };
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct VulkanPipeline {
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        // std::vector<DrawInfo> renderPool;
    };

    struct VulkanPipelineOptions {};

    struct VulkanBuffer {
        VkBuffer buffer;
    };

#define ALLOCATED_VERTICES 512
#define ALLOCATED_INDICES 2048

    class PRESTO_API VulkanRenderer : public Renderer {
       public:
        VulkanRenderer(GLFWwindow* window);
        virtual ~VulkanRenderer();

        virtual void AddToRenderPool(entity_t entity_ptr) override;
        virtual void draw(entity_t entity_ptr) override;
        virtual void nextFrame() override;

       private:
        std::map<entity_t, DrawInfo> _entityMap;
        uint32_t _imageIndex = 0;
        bool _startedDrawing = false;
        GLFWwindow* _glfwWindow;
        std::vector<VulkanPipeline> _graphicsPipelines;

        virtual void Shutdown();

        void drawFromInfo(DrawInfo& info);

        PR_RESULT startRecording(VkCommandBuffer commandBuffer);
        void stopRecording(VkCommandBuffer& commandBuffer);

        /*void drawPipelineToBuffer(VkCommandBuffer commandBuffer,
                                  const VulkanPipeline pipeline);*/

        // Background members
        glm::mat4 getProjectionMatrix(glm::float32 fovRad);

        VkInstance _instance;
        VkDebugUtilsMessengerEXT _debugMessenger;
        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice _logicalDevice;

        VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
        VkFormat _swapchainImageFormat;
        VkExtent2D _swapchainExtent;

        VkRenderPass _renderPass;

        VkDescriptorPool _descriptorPool;
        VkDescriptorSetLayout _descriptorSetLayout;

        std::vector<VkImage> _swapchainImages;
        std::vector<VkImageView> _swapchainImageViews;
        std::vector<VkFramebuffer> _swapchainFramebuffers;

        VkQueue _graphicsQueue;
        VkQueue _presentQueue;
        VkSurfaceKHR _surface;

        VkCommandPool _commandPool;
        std::vector<VkCommandBuffer> _commandBuffers;

        VkBuffer _stagingBuffer;
        VkDeviceMemory _stagingBufferMemory;

        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;

        VkBuffer _indexBuffer;
        VkDeviceMemory _indexBufferMemory;

        std::vector<VkBuffer> _uniformBuffers;
        std::vector<VkDeviceMemory> _uniformBuffersMemory;
        std::vector<void*> _uniformBuffersMapped;

        std::vector<VkDescriptorSet> _descriptorSets;

        std::vector<VkSemaphore> _imageAvailableSemaphores;
        std::vector<VkSemaphore> _renderFinishedSemaphores;
        std::vector<VkFence> _inFlightFences;

        uint32_t _currentFrame = 0;

        uint32_t findMemoryType(uint32_t typeFilter,
                                VkMemoryPropertyFlags properties);

        bool isDeviceSuitable(const VkPhysicalDevice& device);
        QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);

        // High level init functions
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();

        void createRenderPass();
        void createFrameBuffers();
        void createCommandPool();
        void createBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        // void initialiseBuffers();
        void createCommandBuffers();
        void createSyncObjects();

        void createDescriptorSetLayout();
        void buildGraphicsPipeline(VulkanPipeline& pipeline);
        size_t createGraphicsPipeline(VulkanPipelineOptions& options);

        void createDefaultPipeline();

        void cleanupSwapChain();
        void recreateSwapChain();

        // Low level init functions
        VkApplicationInfo makeApplicationInfo();
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags,
                          VkMemoryPropertyFlags propFlags, VkBuffer& buffer,
                          VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize bufferSize);

        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();
        bool checkDeviceExtensionSupport(const VkPhysicalDevice& device) const;
        void initialiseVulkanExtensions();

        VkShaderModule createShaderModule(const std::string& filepath);
        VkShaderModule createShaderModule(const std::vector<char>& code);

        VkPipelineRasterizationStateCreateInfo makeRasterizerInfo();

        SwapChainSupportDetails querySwapChainSupport(
            VkPhysicalDevice device) const;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(
            const VkSurfaceCapabilitiesKHR& capabilities);

        // Validation functions

        void populateDebugMessengerCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        // Ping Vulkan for creation function
        static VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);

        static void DestroyDebugUtilsMessengerEXT(
            VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator);

        // Debug callback function for Vulkan
        static VKAPI_ATTR VkBool32 VKAPI_CALL
        debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                      void* pUserData);
    };
}  // namespace Presto