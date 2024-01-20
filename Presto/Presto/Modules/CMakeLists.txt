target_sources(Presto
PUBLIC	Module.h
		RenderingModule/_GraphicsRenderer.h
		# RenderingModule/_GraphicsRenderer.cpp
		RenderingModule/VulkanRenderer.h
PRIVATE RenderingModule/VulkanRenderer.cpp

PUBLIC	ResourcesModule/ResourceManager.h
		ResourcesModule/ResourceManager.cpp
)

target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})