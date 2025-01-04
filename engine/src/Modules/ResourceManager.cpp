#include "Presto/Modules/ResourceManager.h"

// #include "PrestoCore/Rendering/Mesh.h"

namespace Presto {
    // const fs::path executableDirectory =
    // fs::absolute(fs::path(argv[0])).parent_path();

    // TODO: Fix this to be based on the executable path rather than the
    // directory which the program was called from

    /*
        Mesh* ResourceManager::LoadMesh(const std::string& filepath) {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string warn;
            std::string err;

            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                  filepath.c_str())) {
                throw std::runtime_error(warn + err);
            }

            if (shapes.size() == 0) {
                return nullptr;
            }

            for (const auto& shape : shapes) {
                for (const auto& index : shape.mesh.indices) {
                    Vertex vertex{};

                    vertices.push_back(vertex);
                    indices.push_back(indices.size());
                }
            }
        }
            */

}  // namespace Presto
