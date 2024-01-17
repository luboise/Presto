#include "_GraphicsRenderer.h"

#include <vulkan/vulkan.h>

namespace Presto {
    class PRESTO_API VulkanRenderer : public RenderingModule {
       public:
        VulkanRenderer();
        ~VulkanRenderer();

        virtual PR_RESULT Init();
        virtual void Shutdown();

       private:
        VkInstance _instance;

        bool checkValidationLayerSupport();
        void initialiseVulkanExtensions();
    };
}  // namespace Presto