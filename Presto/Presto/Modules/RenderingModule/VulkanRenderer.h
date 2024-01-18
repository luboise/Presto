#include "_GraphicsRenderer.h"

#include <vulkan/vulkan.h>

namespace Presto {
    class PRESTO_API VulkanRenderer : public RenderingModule {
       public:
        VulkanRenderer();
        ~VulkanRenderer();

        virtual void Init();
        virtual void Shutdown();

       private:
        VkInstance _instance;
        VkDebugUtilsMessengerEXT _debugMessenger;

        // High level init functions
        PR_RESULT createInstance();
        PR_RESULT setupDebugMessenger();

        // Low level init functions
        VkApplicationInfo makeApplicationInfo();

        void populateDebugMessengerCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();
        void initialiseVulkanExtensions();

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