#pragma once

#include "Presto/Module.h"
#include "Scene.h"

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace Presto {
    class SceneManager : public Module {
        MODULE_FUNCTIONS();

       public:
        id_t loadScene(const json& file);
        id_t switchScene(Scene);

       private:
        Scene* _currentScene;
    };
}  // namespace Presto
