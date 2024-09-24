#pragma once

#include "Presto/Components/Renderable.h"

#include "Presto/Core.h"
#include "Presto/Rendering/Renderer.h"
#include "Rendering/Vulkan/Abstractions/Pipeline.h"
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanTypes.h"

namespace Presto {
    using BufferList = std::vector<Buffer*>;
    using BUFFER_TYPE = Buffer::BUFFER_TYPE;

    class BufferManager {
       public:
        explicit BufferManager(VulkanDevice* device);

        Buffer* createUniformBuffer(VkDeviceSize size) {
            auto* ptr = new Buffer(*_device, BUFFER_TYPE::UNIFORM, size);

            this->_uniformBuffers.push_back(ptr);
            return ptr;
        };

        void loadRenderable(Renderable*, Pipeline*);

        const VulkanDrawInfo& getDrawInfo(const draw_info_key key) const {
            PR_CORE_ASSERT(
                _drawInfoMap.contains(key),
                "Unable to get draw info for renderable {} because it has not "
                "been loaded yet. Please call addToRenderPool(ptr) first.",
                fmt::ptr(key));

            return _drawInfoMap.at(key);
        };

       private:
        std::map<draw_info_key, VulkanDrawInfo> _drawInfoMap;

        VulkanDevice* _device;

        CommandPool* _commandPool;

        Buffer* _stagingBuffer;

        BufferList _vertexBuffers;
        BufferList _indexBuffers;

        BufferList _uniformBuffers;
    };
}  // namespace Presto
