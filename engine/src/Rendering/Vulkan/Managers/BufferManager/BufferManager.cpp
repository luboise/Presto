#include "Rendering/Vulkan/Managers/BufferManager/BufferManager.h"

#include "Presto/Components/Renderable.h"
#include "Rendering/Vulkan/Abstractions/Buffer.h"
#include "Rendering/Vulkan/Abstractions/Pipeline.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanTypes.h"

#include "Rendering/Vulkan/VulkanUtils/VulkanConstants.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanVertex.h"

namespace Presto {

BufferManager::BufferManager(VulkanDevice* device)
    : _device(device), _commandPool(_device->createCommandPool()) {
    auto vertexBufferSize = sizeof(VulkanVertex) * ALLOCATED_VERTICES;

    _stagingBuffer = (_device->createBuffer(Buffer::STAGING, vertexBufferSize));
    _vertexBuffers.push_back(
        (_device->createBuffer(Buffer::VERTEX, vertexBufferSize)));
    _indexBuffers.push_back((_device->createBuffer(
        Buffer::INDEX, sizeof(uint32_t) * ALLOCATED_INDICES)));
}

void BufferManager::loadRenderable(Renderable* renderable_ptr,
                                   Pipeline* pipeline) {
    if (_drawInfoMap.contains(renderable_ptr)) {
        PR_CORE_ERROR(
            "Entity has already been added to render pool. Can't add it "
            "again.");
        return;
    }

    // ?? Unsure why this is here
    // Pipeline& pipeline = _graphicsPipelines[0];

    const auto mesh = renderable_ptr->getMesh();
    const VertexList& vertices = mesh.getVertices();
    const IndexList& indices = mesh.getIndices();

    // TODO: Make this actually allocate new space for the object instead of
    // overwriting the previous one
    auto* vertex_buffer = _vertexBuffers[0];
    auto vertex_offset = 0;

    auto* index_buffer = _indexBuffers[0];
    auto index_offset = 0;

    _stagingBuffer->write(sizeof(VulkanVertex) * vertices.size(),
                          vertices.data());
    Buffer::copyInto(*_commandPool, *_stagingBuffer, *vertex_buffer);

    _stagingBuffer->write(sizeof(uint32_t) * indices.size(), indices.data());

    Buffer::copyInto(*_commandPool, *_stagingBuffer, *index_buffer);

    // TODO: Add an allocator here for multiple objects
    auto info = VulkanDrawInfo{};

    info.vertex_buffer = vertex_buffer;
    info.vertex_count = vertices.size();
    info.vertex_offset = vertex_offset;

    info.index_buffer = index_buffer;
    info.index_count = indices.size();
    info.index_offset = index_offset;

    info.pipeline = pipeline;

    auto insertion = _drawInfoMap.try_emplace(renderable_ptr, info);

    PR_CORE_ASSERT(insertion.second,
                   "Presto failed to insert renderable {} to the render list.",
                   fmt::ptr(renderable_ptr));

    PR_CORE_TRACE("Added {} to the render list.", fmt::ptr(renderable_ptr));
};
}  // namespace Presto
