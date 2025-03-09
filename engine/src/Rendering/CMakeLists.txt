# add_subdirectory(Vulkan)
add_subdirectory(OpenGL)

target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE BoundingBox.cpp
          Renderer.cpp
          Utils/RenderingUtils.cpp
          RenderTypes.cpp
          VertexAttribute.cpp
          Shader.cpp
          Pipeline.cpp
          PipelineTypes.cpp
          PipelineBuilder.cpp
          UniformTypes.cpp
          Buffer.cpp
          # TextureInstance.cpp
          # Materials
          ShaderTypes.cpp
          MaterialInstance.cpp)
