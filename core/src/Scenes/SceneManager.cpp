#include "SceneManager.h"
#include "PrestoCore/Log.h"

namespace Presto {
    Scene* SceneManager::_currentScene = nullptr;
    std::map<scene_id_t, Scene*> SceneManager::_sceneMap;

    void SceneManager::Init() {};

    scene_id_t SceneManager::LoadScene(const json& j) {
        if (!j.contains("name") || !j.contains("objects")) {
            return "";
            return Scene::INVALID;
        }

        const auto& scene_name = j["name"];
        const auto& objects = j["objects"];

        PR_CORE_TRACE("Scene: {}", scene_name);
        for (const auto& object : objects) {
            // TODO: Implement this
            PR_CORE_TRACE(object);
        }

        return Scene::INVALID;
    };

    void SceneManager::SwitchScene(const scene_id_t& scene) { return; }
}  // namespace Presto
