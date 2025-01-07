#include "GLTF.h"

namespace Presto {
    GLTF::GLTF(const fs::path& filepath) {
        auto file = Utils::File::ReadFile(filepath);
        auto gltf = Utils::File::GetJSON(file);

        fs::path containing_folder = filepath.parent_path();

        for (const auto& buffer : gltf["buffers"]) {
            auto buffer_filepath = containing_folder / buffer["uri"];
            buffers_.push_back(Utils::File::ReadBinaryFile(buffer_filepath));
        }

        for (const auto& mesh : gltf["meshes"]) {
            auto* new_mr{new MeshResource()};

            for (const auto& primitive : mesh["primitives"]) {
                // auto attributes = primitive["attributes"];
                auto indices_accessor_index = primitive["indices"];
                auto indices_accessor =
                    gltf["accessors"][indices_accessor_index];

                auto indices_bv_index = indices_accessor["bufferView"];
                auto indices_bv = gltf["bufferViews"][indices_bv_index];

                auto starting_offset = (uint)indices_bv["byteOffset"] +
                                       (uint)indices_accessor["byteOffset"];
                for (int i = indices_bufferview["byteOffset"]; i <) }
        }

        ;
    }
}  // namespace Presto
