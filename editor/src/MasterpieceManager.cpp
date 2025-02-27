#include "MasterpieceManager.h"
#include "EditorUI.h"

#include "Modules/AssetManager.h"
#include "Modules/SceneManager.h"
#include "Presto/Utils/File.h"

void MasterpieceManager::Init() {
    MasterpieceManager::instance_ = std::make_unique<MasterpieceManager>();
}

void MasterpieceManager::Shutdown() { MasterpieceManager::instance_.reset(); }

void MasterpieceManager::newMasterpiece() {}

bool MasterpieceManager::openMasterpiece(const fs::path& projectFilepath) {
    if (projectFilepath.extension() != PR_MASTERPIECE_EXTENSION) {
        EditorUI::errorPopup(std::string("Invalid file: ") +
                             projectFilepath.string());
        return false;
    }

    json project_file = Presto::Utils::File::GetJSON(projectFilepath);
    if (project_file == json{}) {
        EditorUI::errorPopup("Error reading masterpiece file.");
    }

    fs::path project_directory = projectFilepath.parent_path();

    // Search the project path for all assets and load those into the scene
    // manager and asset manager
    MasterpieceFileSet project_files{
        getMasterpieceFilesInDirectory(project_directory)};

    // Process all mesh libraries
    for (const fs::path& library_path : project_files.mesh_libraries) {
        json mesh_db_json = Presto::Utils::File::GetJSON(library_path);

        if (mesh_db_json.empty()) {
            EditorUI::errorPopup(
                std::format("Error opening file {}", library_path.string()));
            continue;
        };

        for (const auto& [mesh_name, mesh_data] : mesh_db_json.items()) {
            try {
                if (!mesh_data.contains("path")) {
                    EditorUI::errorPopup(std::format(
                        "Error loading mesh {}. Unable to find key {}",
                        mesh_name, "resource"));
                    continue;
                }

                fs::path resource_path = mesh_data["path"];

                Presto::AssetManager::Get().loadMeshesFromDisk(
                    project_directory / resource_path, mesh_name);

            } catch (...) {
                EditorUI::errorPopup(
                    std::format("Unable to load mesh {}.", mesh_name));
            }
        }
    }

    // Process all scenes
    for (const fs::path& path : project_files.scenes) {
        auto json_data = Presto::Utils::File::GetJSON(path);
        Scene* new_scene{
            Presto::SceneManager::Get().newSceneFromJson(json_data)};

        if (new_scene == nullptr) {
            EditorUI::errorPopup(std::format(
                "Error reading scene {} from JSON.", path.string()));
            continue;
        }

        sceneMap_.emplace(new_scene->getName(), new_scene);
    }

    // Open the main scene of the project
    Masterpiece p{};

    if (auto it = project_file.find("name"); it != project_file.end()) {
        p.name = *it;
    } else {
        EditorUI::errorPopup("Unable to find name in masterpiece file.");
        return false;
    }

    if (auto it = project_file.find("main_scene"); it != project_file.end()) {
        p.main_scene = *it;
    } else {
        EditorUI::errorPopup("Unable to find main_scene in masterpiece file.");
        return false;
    }

    if (Presto::SceneManager::Get().getScene(p.main_scene) == nullptr) {
        EditorUI::errorPopup(
            "The main scene file was not found in the project files.");
        return false;
    }

    currentMasterpiece_ = p;
    // TODO: Set the name of the project to the title bar of the application

    return true;
}

MasterpieceManager::MasterpieceFileSet
MasterpieceManager::getMasterpieceFilesInDirectory(const fs::path& directory) {
    MasterpieceFileSet mfs;

    for (const auto& file : fs::recursive_directory_iterator(directory)) {
        const fs::path& path = file.path();
        if (fs::is_regular_file(path)) {
            if (!path.has_extension()) {
                continue;
            }

            auto extension = path.extension();
            if (extension == PR_SCENE_EXTENSION) {
                mfs.scenes.push_back(path);
            } else if (extension == PR_MESH_LIB_EXTENSION) {
                mfs.mesh_libraries.push_back(path);
            } else {
                mfs.others.push_back(path);
            }
        }
    }

    return mfs;
};

void MasterpieceManager::clearMasterpiece() { sceneMap_.clear(); };

Scene* MasterpieceManager::getMainScene() const {
    auto search = sceneMap_.find(currentMasterpiece_.main_scene);

    if (search == sceneMap_.end()) {
        return nullptr;
    }

    return search->second;
};
