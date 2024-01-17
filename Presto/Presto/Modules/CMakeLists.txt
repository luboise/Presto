target_sources(Presto
PUBLIC	RenderingModule/_GraphicsRenderer.h
		RenderingModule/_GraphicsRenderer.cpp
		RenderingModule/VulkanRenderer.h
		RenderingModule/VulkanRenderer.cpp)

target_include_directories(Presto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})