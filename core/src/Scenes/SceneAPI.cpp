#include "PrestoCore/Scenes.h"

#include "ResourcesModule/ResourceManager.h"
#include "Scenes/SceneManager.h"

namespace Presto {

    std::string Scenes::Load(const std::string& filepath) {
        json json_data =
            ResourceManager::GetJSON(ResourceManager::ReadFile(filepath));

        auto id = SceneManager::LoadScene(json_data);
        if (id == Scene::INVALID) {
            PR_CORE_ERROR("Could not load scene from {}.", filepath);
        }

        return id;
    };

    void Scenes::SwitchTo(const std::string& name) {
        SceneManager::SwitchScene(name);
    }
}  // namespace Presto
