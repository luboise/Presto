target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE # General
          CameraComponent.cpp
          TransformComponent.cpp
          # Rendering
          RenderComponent.cpp
          # Renderable/SpriteSubcomponent.cpp
          # Physics
          Physics/RigidBodyComponent.cpp
          # UI
          UI/CanvasItem.cpp
          UI/CanvasButton.cpp
          UI/CanvasComponent.cpp)
