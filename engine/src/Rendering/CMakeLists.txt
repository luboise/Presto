# add_subdirectory(Vulkan)
add_subdirectory(OpenGL)

target_sources(
  PrestoEngine PRIVATE BoundingBox.cpp Renderer.cpp Utils/RenderingUtils.cpp
                       RendererFactory.cpp RenderTypes.cpp Material.cpp)
