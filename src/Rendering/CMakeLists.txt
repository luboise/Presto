target_sources(
		Presto
		PRIVATE Vulkan/VulkanRenderer.cpp
										Vulkan/VulkanRenderer_draw.cpp
										Vulkan/VulkanRenderer_validation.cpp
										Vulkan/VulkanRenderer_init.cpp
										Vulkan/VulkanRenderer_shading.cpp
										Vulkan/VulkanVertex.cpp
										Types/ModelComponent.cpp
										Types/RenderLayer.cpp
										RenderingManager.cpp)
