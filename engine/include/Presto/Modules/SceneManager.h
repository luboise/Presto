#pragma once

#include "Presto/Objects/Scene.h"

#include "Presto/Modules/Module.h"

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace Presto {
    class SceneManager : public Module<SceneManager> {
       public:
        static void init();
        void update() override {};
        static void shutdown();

        Scene* newScene(std::string name);

        [[nodiscard]] Scene* newSceneFromJson(json jsonData);

        Scene* LoadScene(const json& j);
        void SwitchScene(const scene_name_t&);
        void SwitchScene(Scene&);

        Scene* getScene(const scene_name_t&);

        SceneManager(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager& operator=(SceneManager&&) = delete;

        virtual ~SceneManager() = default;

       private:
        SceneManager();

        // Returns the error as a string if an error occurs when validating the
        // scene
        static std::string validateSceneSyntax(const json& sceneData);

        Scene* currentScene_{nullptr};
        std::map<scene_name_t, std::unique_ptr<Scene>> sceneMap_;
    };

}  // namespace Presto
