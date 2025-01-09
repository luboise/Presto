#include "Presto/API/SceneAPI.h"

#include "Presto/Modules/SceneManager.h"

namespace Presto {

    using namespace Presto::Utils;

    Scene* Scenes::Load(const scene_id_t& filepath) {
        json json_data = File::GetJSON(File::ReadFile(filepath));

        Scene* new_scene = SceneManager::LoadScene(json_data);
        if (new_scene == Scene::INVALID) {
            PR_CORE_ERROR("Could not load scene from {}.", filepath);
        }

        return new_scene;
    };

    void Scenes::SwitchTo(const scene_id_t& name) {
        SceneManager::SwitchScene(name);
    }
}  // namespace Presto
