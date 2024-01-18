target_sources(Presto
PRIVATE	RenderingModule/_GraphicsRenderer.h
		RenderingModule/_GraphicsRenderer.cpp
		RenderingModule/VulkanRenderer.h
		RenderingModule/VulkanRenderer.cpp
PRIVATE	Module.h)

target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})