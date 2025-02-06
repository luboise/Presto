#pragma once

#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/Abstractions/DescriptorPool.h"
#include "Rendering/Vulkan/Abstractions/DescriptorSetLayout.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanTypes.h"

namespace Presto {
class BufferManager;

class DescriptorGroup {
   public:
    DescriptorGroup(DescriptorPool&, BufferManager&,
                    const std::vector<DescriptorSetLayout*>&, uint32_t);
    ~DescriptorGroup();

    [[nodiscard]] const DescriptorFrameSet& getFrameSet(uint32_t) const;
    [[nodiscard]] SetLayoutList getLayouts() const { return _setLayouts; }

   private:
    DescriptorPool* _pool;
    SetLayoutList _setLayouts;

    std::vector<DescriptorFrameSet> _frameSets;
};

}  // namespace Presto
