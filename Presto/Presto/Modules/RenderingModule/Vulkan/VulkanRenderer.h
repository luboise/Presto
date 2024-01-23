#include "Presto/Modules/RenderingModule/_Renderer.h"
#include "VulkanVertex.h"

#include <vulkan/vulkan.h>
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

    const std::vector<VulkanVertex> vertices = {
        // // Tri 1
        // {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        // {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        // {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},

        // // Tri 2
        // {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        // {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        // {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},

        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},

        // Tri 2
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},

    };

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

    class PRESTO_API VulkanRenderer : public Renderer {
       public:
        VulkanRenderer(GLFWwindow* window);
        virtual ~VulkanRenderer();

        virtual void Init();
        virtual void Shutdown();

        PR_RESULT drawFrame();

       private:
        GLFWwindow* _glfwWindow;

        VkInstance _instance;
        VkDebugUtilsMessengerEXT _debugMessenger;
        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice _logicalDevice;

        VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
        VkFormat _swapchainImageFormat;
        VkExtent2D _swapchainExtent;

        std::vector<VkImage> _swapchainImages;
        std::vector<VkImageView> _swapchainImageViews;
        std::vector<VkFramebuffer> _swapchainFramebuffers;

        VkRenderPass _renderPass;
        VkPipelineLayout _pipelineLayout;
        VkPipeline _graphicsPipeline;

        VkQueue _graphicsQueue;
        VkQueue _presentQueue;
        VkSurfaceKHR _surface;

        VkCommandPool _commandPool;
        std::vector<VkCommandBuffer> _commandBuffers;

        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;

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
        void createGraphicsPipeline();
        void createFrameBuffers();
        void createCommandPool();
        void createVertexBuffer();
        void createCommandBuffers();
        void createSyncObjects();

        PR_RESULT recordCommandBuffer(VkCommandBuffer commandBuffer,
                                      uint32_t imageIndex);

        void cleanupSwapChain();
        void recreateSwapChain();

        // Low level init functions
        VkApplicationInfo makeApplicationInfo();

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