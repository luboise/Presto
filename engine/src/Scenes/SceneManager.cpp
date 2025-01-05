#include "SceneManager.h"
#include "PrestoCore/Logging.h"

namespace Presto {
    Scene* SceneManager::_currentScene = nullptr;
    std::map<scene_id_t, Scene*> SceneManager::_sceneMap;

    void SceneManager::Init() {};

    Scene* SceneManager::LoadScene(const json& j) {
        if (!j.contains("name") || !j.contains("objects")) {
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

    void SceneManager::SwitchScene(const scene_id_t& id) {
        Scene* scene_ptr = GetScene(id);
        if (scene_ptr != nullptr) {
            SwitchScene(*scene_ptr);
        }
    }

    // TODO: Make this actually switch out the scene
    void SceneManager::SwitchScene(Scene& scene) { _currentScene = &scene; }

    Scene* SceneManager::GetScene(const scene_id_t& id) {
        auto scene_iterator = _sceneMap.find(id);

        if (scene_iterator == _sceneMap.end()) {
            PR_CORE_WARN("Unable to get scene: {}", id);
            return nullptr;
        }

        return scene_iterator->second;
    };
}  // namespace Presto
