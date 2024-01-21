target_sources(Presto
PUBLIC	Module.h
		RenderingModule/_GraphicsRenderer.h
		# RenderingModule/_GraphicsRenderer.cpp
		RenderingModule/VulkanRenderer.h
PRIVATE RenderingModule/VulkanRenderer.cpp
		RenderingModule/VulkanRenderer_draw.cpp
		RenderingModule/VulkanRenderer_validation.cpp
		RenderingModule/VulkanRenderer_init.cpp
		RenderingModule/VulkanRenderer_shading.cpp

PUBLIC	ResourcesModule/ResourceManager.h
PRIVATE	ResourcesModule/ResourceManager.cpp
)

target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})