#include "Components/ConductorComponent.h"  // IWYU pragma: export

#include "Components/Physics/RigidBodyComponent.h"  // IWYU pragma: export

#include "Components/Renderable/ModelComponent.h"   // IWYU pragma: export
#include "Components/Renderable/SpriteComponent.h"  // IWYU pragma: export

#include "Components/CameraComponent.h"     //IWYU pragma: export
#include "Components/TransformComponent.h"  // IWYU pragma: export

using Camera = Presto::CameraComponent;
using Conductor = Presto::ConductorComponent;
using Model = Presto::ModelComponent;
using RigidBody = Presto::RigidBodyComponent;
using Sprite = Presto::SpriteComponent;
using Transform = Presto::TransformComponent;
