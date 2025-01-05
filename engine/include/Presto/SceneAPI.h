#include "Presto/Scenes/Scene.h"

class Scene;

namespace Presto::Scenes {
    Scene* Load(const std::string& filepath);
    void SwitchTo(const std::string& name);
}  // namespace Presto::Scenes
