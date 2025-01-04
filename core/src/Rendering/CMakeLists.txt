# add_subdirectory(Vulkan)
add_subdirectory(OpenGL)

target_sources(PrestoCore PRIVATE Utils/RenderingUtils.cpp RendererFactory.cpp)
