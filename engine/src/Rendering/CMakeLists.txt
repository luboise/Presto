# add_subdirectory(Vulkan)
add_subdirectory(OpenGL)

target_sources(PrestoEngine PRIVATE Utils/RenderingUtils.cpp
                                    RendererFactory.cpp Material.cpp)
