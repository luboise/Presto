#pragma once

#include "Rendering/Renderer.h"

/*
#include "Abstractions/Buffer.h"
#include "Abstractions/Swapchain.h"
*/

#include "Managers/DrawManager/DrawManager.h"
// #include "Managers/PipelineManager/PipelineManager.h"

// KEEP THESE SEPARATE
#include "Managers/BufferManager/BufferManager.h"
#include "Managers/DescriptorManager/DescriptorManager.h"
#include "Rendering/Vulkan/VulkanContext.h"

#include <GLFW/glfw3.h>

namespace Presto {

class GLFWAppWindow;
class PipelineManager;

class VulkanRenderer final : public Renderer {
   public:
    explicit VulkanRenderer(GLFWAppWindow* window);
    ~VulkanRenderer() override;

    void render(MeshRegistrationData& data) override;

    AllocatedPipelineList createDefaultPipelines() override;
    Allocated<PipelineBuilder> getPipelineBuilder() override;

    Allocated<TextureFactory> getTextureFactory() override;

    Allocated<Buffer> createBuffer(Buffer::BufferType type,
                                   Presto::size_t size) override;

    Allocated<UniformBuffer> createUniformBuffer(Presto::size_t size) override;

    void nextFrame() override;

    bool createMeshContext(MeshRegistrationData& registration,
                           const PipelineStructure& structure) override;

    [[nodiscard]] static VulkanDevice& getDevice();

   private:
    static inline Allocated<VulkanDevice> device_ = nullptr;

    VulkanContext context;

    Allocated<DrawManager> drawManager_ = nullptr;
    Allocated<PipelineManager> pipelineManager_ = nullptr;
    Allocated<BufferManager> bufferManager_ = nullptr;
    Allocated<DescriptorManager> descriptorManager_ = nullptr;

    void initialiseSystems();
    void Shutdown();

    DrawManager::VulkanDrawContext _drawContext{};

    void onFrameBufferResized() override { this->drawManager_->queueReload(); }

    // Background members
    std::vector<Allocated<Pipeline>> pipelines_;

    VkSurfaceKHR surface_{};

    // Available graphics cards
    std::vector<VkPhysicalDevice> physicalDevices_;

    // The current physical device in use by the renderer
    // (assumed to always be only 1)

    VkDebugUtilsMessengerEXT debugMessenger_{};

    Allocated<Swapchain> swapchain_ = nullptr;

    static bool isDeviceSuitable(const VkPhysicalDevice&, const VkSurfaceKHR&);

    [[nodiscard]] VkSurfaceKHR createSurface() const;

    static std::vector<VkPhysicalDevice> getPhysicalDevices(
        const VkInstance&, const VkSurfaceKHR&);

    void createVulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags,
                            VkMemoryPropertyFlags propFlags, VkBuffer& buffer,
                            VkDeviceMemory& bufferMemory);
};
}  // namespace Presto
