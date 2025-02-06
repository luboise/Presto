target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE OpenGLRenderer.cpp
          utils.cpp
          OpenGLBuffer.cpp
          OpenGLPipeline.cpp
          OpenGLShader.cpp
          OpenGLDrawManager/OpenGLDrawManager.cpp
          OpenGLDrawManager/OpenGLTexture.cpp)
