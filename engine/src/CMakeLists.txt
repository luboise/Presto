add_subdirectory(Common)
add_subdirectory(Components)
add_subdirectory(Modules)
add_subdirectory(Objects)
add_subdirectory(Rendering)
add_subdirectory(Shaders)
add_subdirectory(Utils)

target_precompile_headers(PrestoEngine PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)

target_sources(
  PrestoEngine
  PRIVATE Application.cpp
          Rendering/RenderLayer.cpp
          Rendering/RenderingManager.cpp
          Rendering/Camera.cpp
          Runtime/Time.cpp
          Logging.cpp
          pch.cpp
          GLFWAppWindow.cpp)

# add_subdirectory(Platform)
