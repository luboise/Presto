# add_subdirectory(Vulkan)
add_subdirectory(OpenGL)

target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE BoundingBox.cpp
          Renderer.cpp
          Utils/RenderingUtils.cpp
          RendererFactory.cpp
          RenderTypes.cpp
          VertexAttribute.cpp
          Shader.cpp
          PipelineTypes.cpp
          PipelineBuilder.cpp)
