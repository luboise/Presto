target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE Application.cpp
          Materials.cpp
          Events.cpp
          Objects.cpp
          Math.cpp
          Physics.cpp
          pch.cpp
          System.cpp
          Collision.cpp
          Debugging.cpp
          GLFWAppWindow.cpp)
