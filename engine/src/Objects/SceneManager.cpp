#include "Presto/Modules/SceneManager.h"

namespace Presto {

    SceneManager::SceneManager() = default;

    void SceneManager::Init() {
        instance_ = std::unique_ptr<SceneManager>(new SceneManager());
    };

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

    void SceneManager::SwitchScene(const scene_name_t& id) {
        Scene* scene_ptr = getScene(id);
        if (scene_ptr != nullptr) {
            SwitchScene(*scene_ptr);
        }
    }

    // TODO: Make this actually switch out the scene
    void SceneManager::SwitchScene(Scene& scene) { currentScene_ = &scene; }

    Scene* SceneManager::getScene(const scene_name_t& id) {
        auto scene_iterator = sceneMap_.find(id);

        if (scene_iterator == sceneMap_.end()) {
            PR_CORE_WARN("Unable to get scene: {}", id);
            return nullptr;
        }

        return scene_iterator->second.get();
    };

    Scene* SceneManager::newScene(std::string name) {
        sceneMap_.emplace(name, std::make_unique<Scene>(name));

        return sceneMap_[name].get();
    };

    Scene* SceneManager::newSceneFromJson(json jsonData) {
        try {
            scene_name_t name = jsonData["name"];

            auto new_scene{std::make_unique<Scene>(name)};

            sceneMap_.emplace(name, std::move(new_scene));
            return getScene(name);

        } catch (std::exception& e) {
            PR_CORE_ERROR("Unable to create scene from json data.");
        }

        return nullptr;
    };
}  // namespace Presto
