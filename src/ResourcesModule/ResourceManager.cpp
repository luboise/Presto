#include "ResourceManager.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include "Presto/Constants.h"

// #include "Presto/Rendering/Mesh.h"

namespace fs = std::filesystem;

namespace Presto {
    // const fs::path executableDirectory =
    // fs::absolute(fs::path(argv[0])).parent_path();

    // TODO: Fix this to be based on the executable path rather than the
    // directory which the program was called from
    const fs::path executableDirectory = std::filesystem::current_path();

    std::string ResourceManager::ReadFile(const std::string& filename) {
        // ate <-> start at end of file
        auto filepath = executableDirectory / fs::path(filename);
        PR_CORE_TRACE(filepath.generic_string());
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            PR_CORE_ERROR("Unable to load file \"{}\"", filepath.string());
            return "";
        }

        // Get tellg position(EOF = filesize)
        size_t size = (size_t)file.tellg();
        file.seekg(0);

        std::string buffer(size, ' ');
        file.read(buffer.data(), size);
        file.close();

        return buffer;
    }

    json ResourceManager::GetJSON(const std::string& text) {
        // TODO: Implement this
        try {
            auto parsed = json::parse(text.begin(), text.end());
            return parsed;
        } catch (nlohmann::json::parse_error& error) {
            PR_CORE_ERROR("Unable to parse json file. Error: {}", error.what());
            return {};
        }
    }

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
