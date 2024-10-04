#include "SceneManager.h"
#include "Presto/Log.h"

#include "Presto/Objects/EntityManager.h"
#include "Presto/Rendering/RenderingManager.h"

#include "Presto/Rendering/Meshes/Cube.h"

namespace Presto {
    Scene* SceneManager::_currentScene = nullptr;
    std::map<scene_id_t, Scene*> SceneManager::_sceneMap;

    void SceneManager::Init() {};

    scene_id_t SceneManager::LoadScene(const json& j) {
        if (!j.contains("name")) {
            PR_CORE_WARN(
                "Imported scene is missing a name attribute. Likely not a "
                "valid scene file.");
            return Scene::INVALID;
        }

        if (!j.contains("objects")) {
            PR_CORE_WARN(
                "Imported scene is missing an objects array. Likely not a "
                "valid scene file.");
            return Scene::INVALID;
        }

        const auto& scene_name = j["name"];
        const auto& objects = j["objects"];

        PR_CORE_TRACE("Loading scene: {}", scene_name);
        Scene* scene = new Scene();

        for (const auto& object : objects) {
            // TODO: Implement this
            if (object == "Cube") {
                // this->_heart = Pr::EntityManager::newEntity();
                auto* cube_entity = EntityManager::newEntity();

                glm::vec3 colour = {0, 0, 1};

                auto thingos = Meshes::Cube({-1, -1, -1}, 0.3, colour);

                auto& mgr = RenderingManager::Get();

                auto* mesh = mgr.NewMesh(thingos.vertices, thingos.indices);
                auto* props = mgr.NewRenderableProps();
                auto* renderable = mgr.NewRenderable(*mesh, *props);

                cube_entity->setComponent(renderable);
                mgr.AddRenderable(0, renderable);

                scene->addEntity(cube_entity);
            } else {
                auto error_msg = std::format("Unimplemented object: {}",
                                             std::string(object));
                PR_CORE_ERROR(error_msg);
                throw std::runtime_error(error_msg);
            }
            PR_CORE_TRACE(object);
        }

        return scene->getId();
    };

    void SceneManager::SwitchScene(const scene_id_t& scene) { return; }
}  // namespace Presto
