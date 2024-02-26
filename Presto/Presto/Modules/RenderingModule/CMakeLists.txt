target_sources(Presto
PUBLIC	_Renderer.h
		RenderingManager.h

		# _Renderer.cpp
		Vulkan/VulkanRenderer.h
		Vulkan/VulkanVertex.h

		Types/ModelComponent.h

PRIVATE Vulkan/VulkanRenderer.cpp
		Vulkan/VulkanRenderer_draw.cpp
		Vulkan/VulkanRenderer_validation.cpp
		Vulkan/VulkanRenderer_init.cpp
		Vulkan/VulkanRenderer_shading.cpp
		Vulkan/VulkanVertex.cpp
		
		RenderTypes.h

		Types/ModelComponent.cpp
		Types/RenderLayer.h
		Types/RenderLayer.cpp

		RenderingManager.cpp
)