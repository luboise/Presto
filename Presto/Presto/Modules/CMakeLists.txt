target_sources(Presto
PUBLIC	RenderingModule/_GraphicsRenderer.h
		# RenderingModule/_GraphicsRenderer.cpp
PUBLIC	RenderingModule/VulkanRenderer.h
PRIVATE RenderingModule/VulkanRenderer.cpp
PUBLIC	Module.h)

target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})