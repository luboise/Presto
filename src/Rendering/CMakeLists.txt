add_subdirectory(Vulkan)
add_subdirectory(OpenGL)

target_sources(Presto PRIVATE RenderLayer.cpp RenderingManager.cpp
                              Renderable.cpp Mesh.cpp Utils/RenderingUtils.cpp)
