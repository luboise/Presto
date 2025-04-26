target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE Abstractions/OpenGLTextures.cpp
          OpenGLRenderer.cpp
          utils.cpp
          OpenGLBuffer.cpp
          OpenGLPipeline.cpp
          OpenGLVAO.cpp
          OpenGLUniformBuffer.cpp)
