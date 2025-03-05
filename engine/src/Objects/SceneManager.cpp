#include "Modules/SceneManager.h"
#include "Presto/Objects/Components/Renderable/ModelComponent.h"

#include "Modules/AssetManager.h"
#include "Modules/EntityManagerImpl.h"

namespace Presto {
constexpr auto TYPE_KEY = "type";
constexpr auto NAME_KEY = "name";

constexpr auto ENTITY_KEY = "entity";

constexpr auto OBJECTS_KEY = "objects";
constexpr auto COMPONENTS_KEY = "components";

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() { this->sceneMap_.clear(); };

Scene* SceneManager::LoadScene(const json& j) {
    if (!j.contains(NAME_KEY) || !j.contains(OBJECTS_KEY)) {
        return Scene::INVALID;
    }

    const auto& scene_name = j[NAME_KEY];
    const auto& objects = j[OBJECTS_KEY];

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
        return Scene::INVALID;
    }

    return scene_iterator->second.get();
};

Scene* SceneManager::newScene(std::string name) {
    sceneMap_.emplace(name, std::make_unique<Scene>(name));

    return sceneMap_[name].get();
};

Scene* SceneManager::newSceneFromJson(json jsonData) {
    if (auto message = validateSceneSyntax(jsonData); message != "") {
        PR_CORE_ERROR("Error validating scene: {}", message);
        return Scene::INVALID;
    }

    scene_name_t scene_name = jsonData[NAME_KEY];
    auto new_scene{std::make_unique<Scene>(scene_name)};

    for (const auto& object : jsonData[OBJECTS_KEY]) {
        if (object[TYPE_KEY] == ENTITY_KEY) {
            auto entity_name = object[NAME_KEY];
            EntityPtr new_entity =
                EntityManagerImpl::get().newEntity(entity_name);

            const auto& components = object[COMPONENTS_KEY];

            for (const auto& [componentKey, component] : components.items()) {
                if (componentKey == "mesh") {
                    auto mesh_data = components[componentKey];

                    ModelPtr mr{AssetManager::get().find<AssetType::MESH>(
                        component["resource"])};

                    if (mr == nullptr) {
                        PR_CORE_ERROR("Unable to find mesh resource: {}",
                                      component["resource"]);
                        return Scene::INVALID;
                    }

                    ComponentPtr new_mesh_component{
                        EntityManagerImpl::get().newComponent<ModelComponent>(
                            mr)};

                    new_entity->setComponent(new_mesh_component);
                }
            }

            new_scene->addEntity(new_entity);
        }
    }
    sceneMap_.emplace(scene_name, std::move(new_scene));
    return getScene(scene_name);
}

std::string SceneManager::validateSceneSyntax(const json& sceneData) {
    auto hasKeys{[](const json& data,
                    std::initializer_list<std::string> keys) -> std::string {
        for (const std::string& key : keys) {
            if (!data.contains(key)) {
                return std::format("Unable to find key \"{}\" in json object.",
                                   key, data.type_name());
            }
        }

        return "";
    }};

    auto validateEntity{[hasKeys](const json& entityData) -> std::string {
        if (auto message = hasKeys(entityData, {NAME_KEY, COMPONENTS_KEY});
            message != "") {
            return message;
        }

        const auto& componentsDict{entityData[COMPONENTS_KEY]};

        if (!componentsDict.is_object()) {
            return std::format("Key {} exists, but is not a JSON object.",
                               COMPONENTS_KEY);
        }

        for (const auto& [componentKey, component] : componentsDict.items()) {
            // TODO: Implement individual checks here
            if (componentKey == "mesh") {
                // Check that all meshes have a resource
                if (auto message = hasKeys(component, {"resource"});
                    message != "") {
                    return message;
                }
            } else if (componentKey == "transform") {
            } else {
                return std::format("Invalid component type: {}.",
                                   std::string(componentKey));
            }
        }

        return "";
    }};

    // Check for name and objects keys
    if (auto message = hasKeys(sceneData, {NAME_KEY, OBJECTS_KEY});
        message != "") {
        return message;
    }

    const auto& objects = sceneData[OBJECTS_KEY];

    if (!objects.is_array()) {
        return std::format("Key {} appears, but its data is not a JSON array.",
                           OBJECTS_KEY);
    }

    for (const auto& object : objects) {
        hasKeys(object, {TYPE_KEY});

        // If it is a serialised raw entity, validate it as an entity
        if (object[TYPE_KEY] == ENTITY_KEY) {
            if (std::string message = validateEntity(object); message != "") {
                return message;
            }
        } else {
            return std::format("Invalid serialised object type: {}",
                               std::string{object[TYPE_KEY]});
        }
    }
    return "";
}
}  // namespace Presto
