# add_subdirectory(Vulkan)
add_subdirectory(OpenGL)

target_sources(PrestoEngine PRIVATE Renderer.cpp Utils/RenderingUtils.cpp
	RendererFactory.cpp )
