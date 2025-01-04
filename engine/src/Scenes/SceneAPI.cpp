#include "Presto/Scenes/Scenes.h"
#include "SceneManager.h"

namespace Presto {

    using namespace Presto::Utils;

    std::string Scenes::Load(const std::string& filepath) {
        json json_data = File::GetJSON(File::ReadFile(filepath));

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
