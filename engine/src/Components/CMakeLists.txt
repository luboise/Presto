target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE Renderable/ModelComponent.cpp
          Renderable/SpriteComponent.cpp
          Physics/RigidBodyComponent.cpp
          CameraComponent.cpp
          TransformComponent.cpp
          # UI
          UI/CanvasItem.cpp
          UI/CanvasButton.cpp
          UI/CanvasComponent.cpp)
