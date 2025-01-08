add_subdirectory(Abstractions)
add_subdirectory(VulkanDevice)
add_subdirectory(Managers)
add_subdirectory(VulkanUtils)

target_sources(
  Presto
  PRIVATE VulkanRenderer.cpp QueueFamilyIndices/QueueFamilyIndices.cpp
          VulkanShader/VulkanShader.cpp)
