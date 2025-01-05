add_subdirectory(Components)
add_subdirectory(Objects)
add_subdirectory(Modules)
add_subdirectory(Scenes)

target_sources(
  PrestoEngine
  PRIVATE Application.cpp Rendering/RenderLayer.cpp
          Rendering/RenderingManager.cpp Rendering/Camera.cpp
          Rendering/Renderable.cpp)
