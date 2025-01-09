#include "ProjectManager.h"
#include <filesystem>
#include "Presto/Objects/Scene.h"
#include "Presto/Utils/File.h"

void MasterpieceManager::Init() {
    MasterpieceManager::instance_ = std::make_unique<MasterpieceManager>();
}

void MasterpieceManager::Shutdown() { MasterpieceManager::instance_.reset(); }

void MasterpieceManager::newProject() {}

void MasterpieceManager::openProject(const fs::path& projectFilepath) {
    if (projectFilepath.extension() != PR_PROJECT_EXTENSION) {
    }

    json file = Presto::Utils::File::GetJSON(projectFilepath);

    // Set the name of the project to the title bar of the application

    // Search the project path for all assets and load those into the scene
    // manager and asset manager

    std::vector<fs::path> project_files =
        getProjectFilesInDirectory(projectFilepath.parent_path());

    // Open the main scene of the project

    Presto::scene_id_t main_scene = file["main_scene"];
}

std::vector<fs::path> MasterpieceManager::getProjectFilesInDirectory(
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
