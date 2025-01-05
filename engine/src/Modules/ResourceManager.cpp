#include "Presto/Modules/ResourceManager.h"
#include "Rendering/Meshes/Cube.h"

// #include "PrestoCore/Rendering/Mesh.h"

namespace Presto {
    // const fs::path executableDirectory =
    // fs::absolute(fs::path(argv[0])).parent_path();

    // TODO: Fix this to be based on the executable path rather than the
    // directory which the program was called from

    MeshResource& ResourceManager::LoadMeshFromDisk(const AssetPath& filepath) {
        // TODO: Implement the actual asset reading of GITF files. For now, this
        // is just stubbed with a unit cube at 0,0,0.

        auto filename{filepath.stem()};

        auto* new_mr{new MeshResource()};

        auto default_cube = Meshes::Cube({0, 0, 0}, 1, {1, 1, 1});

        new_mr->name = filename;

        meshResources_[new_mr->name] = new_mr;
        return *new_mr;
    }

}  // namespace Presto
