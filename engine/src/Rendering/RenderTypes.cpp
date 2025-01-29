#include "Presto/Rendering/RenderTypes.h"

namespace Presto {
    MeshData MeshData::from(const RawMeshData& rawData) {
        MeshData mesh_data;

        mesh_data.indices.resize(rawData.indices.size());

        std::ranges::transform(rawData.indices.begin(), rawData.indices.end(),
                               mesh_data.indices.begin(), [](auto val) {
                                   return static_cast<Index>(val);
                               });

        mesh_data.vertices.resize(
            std::min({rawData.positions.size(), rawData.normals.size(),
                      rawData.tex_coords.size()}));

        for (std::size_t i = 0; i < mesh_data.vertices.size(); i++) {
            Vertex v = {.position = rawData.positions[i],
                        // Default colour of white
                        .colour = {1, 1, 1},
                        .normal = rawData.normals[i],
                        .tex_coords = rawData.tex_coords[i]};
            mesh_data.vertices[i] = v;
        }

        mesh_data.draw_mode = rawData.draw_mode;
        // data.material = rawData.defaultMaterial_;

        return mesh_data;
    }

    BoundingBox MeshData::getBoundingBox() const {
        BoundingBox box;

        for (const auto& vertex : this->vertices) {
            const auto& pos{vertex.position};

            if (pos.x < box.x_min) {
                box.x_min = pos.x;
            } else if (pos.x > box.x_max) {
                box.x_max = pos.x;
            }

            if (pos.y < box.y_min) {
                box.y_min = pos.y;
            } else if (pos.y > box.y_max) {
                box.y_max = pos.y;
            }

            if (pos.z < box.z_min) {
                box.z_min = pos.z;
            } else if (pos.z > box.z_max) {
                box.z_max = pos.z;
            }
        }

        return box;
    };
}  // namespace Presto
