target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE # General
          TransformComponent.cpp
          # Rendering
          Renderables/Renderables.cpp
          # Renderables/SpriteSubcomponent.cpp Physics
          Physics/RigidBodyComponent.cpp)
