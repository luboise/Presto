# add_subdirectory(Vulkan)
target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE BoundingBox.cpp
          Utils/RenderingUtils.cpp
          VertexAttribute.cpp
          Shader.cpp
          Pipeline.cpp
          PipelineTypes.cpp
          PipelineBuilder.cpp
          UniformTypes.cpp
          Buffer.cpp
          # Materials
          ShaderTypes.cpp
          MaterialInstance.cpp
          VertexTypes.cpp)
