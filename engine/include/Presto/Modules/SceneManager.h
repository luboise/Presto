#pragma once

#include "Presto/Objects/Scene.h"

#include "Presto/Modules/Module.h"

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace Presto {
    class SceneManager : public Module<SceneManager> {
       public:
        static void Init();

        static Scene* LoadScene(const json& j);
        static void SwitchScene(const scene_id_t&);
        static void SwitchScene(Scene&);

        static Scene* GetScene(const scene_id_t&);

        SceneManager(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager& operator=(SceneManager&&) = delete;

        virtual ~SceneManager() = delete;

       private:
        SceneManager();

        static Scene* _currentScene;
        static std::map<scene_id_t, Scene*> _sceneMap;
    };

}  // namespace Presto
