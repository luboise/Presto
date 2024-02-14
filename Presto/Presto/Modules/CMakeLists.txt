target_sources(Presto
PUBLIC	Module.h
		RenderingModule/_Renderer.h
		# RenderingModule/_Renderer.cpp
		RenderingModule/Vulkan/VulkanRenderer.h
		RenderingModule/Vulkan/VulkanVertex.h
PRIVATE RenderingModule/Vulkan/VulkanRenderer.cpp
		RenderingModule/Vulkan/VulkanRenderer_draw.cpp
		RenderingModule/Vulkan/VulkanRenderer_validation.cpp
		RenderingModule/Vulkan/VulkanRenderer_init.cpp
		RenderingModule/Vulkan/VulkanRenderer_shading.cpp
		RenderingModule/Vulkan/VulkanVertex.cpp
		RenderingModule/RenderTypes.h

PUBLIC	ResourcesModule/ResourceManager.h
PRIVATE	ResourcesModule/ResourceManager.cpp

PUBLIC 	InputModule/InputManager.h
)

target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})