#include "MasterpieceManager.h"
#include "EditorUI.h"

#include "Presto/Modules/SceneManager.h"
#include "Presto/Utils/File.h"

#include <filesystem>

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

    // Set the name of the project to the title bar of the application

    // Search the project path for all assets and load those into the scene
    // manager and asset manager

    std::vector<fs::path> project_files =
        getMasterpieceFilesInDirectory(projectFilepath.parent_path());

    for (const fs::path& path : project_files) {
        if (path.extension() == PR_SCENE_EXTENSION) {
            auto json_data = Presto::Utils::File::GetJSON(path);
            Scene* new_scene{
                Presto::SceneManager::Get().newSceneFromJson(json_data)};
            sceneMap_.emplace(new_scene->getName(), new_scene);
        }
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

    return true;
}

std::vector<fs::path> MasterpieceManager::getMasterpieceFilesInDirectory(
    const fs::path& directory) {
    std::vector<fs::path> paths;

    for (const auto& file : fs::recursive_directory_iterator(directory)) {
        const fs::path& path = file.path();
        if (fs::is_regular_file(path)) {
            if (!path.has_extension()) {
                continue;
            }

            auto extension = path.extension();

            paths.push_back(path);
        }
    }

    return paths;
};

void MasterpieceManager::clearMasterpiece() { sceneMap_.clear(); };

Scene* MasterpieceManager::getMainScene() const {
    auto search = sceneMap_.find(currentMasterpiece_.main_scene);

    if (search == sceneMap_.end()) {
        return nullptr;
    }

    return search->second;
};
