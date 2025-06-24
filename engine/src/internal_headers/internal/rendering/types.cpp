#include "internal/rendering.h"
import :types;

#include <utility>

namespace Pr {
void MeshData::setVertices(const ImportedAttributeList& attributes) {
    auto processed{processVertices<Vertex3D>(attributes)};

    // TODO: Put checks here to make sure the processed vertices are well
    // formed
    this->vertices = std::move(processed);
}

// Mesh functions
Mesh::Mesh(mesh_registration_id_t id) : registrationId_(id) {};

mesh_registration_id_t Mesh::registrationId() const { return registrationId_; }

/*
BoundingBox MeshData::getBoundingBox() const {
    BoundingBox box;

    for (const auto& vertex : this->vertex_data.data) {
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
*/

[[nodiscard]] Pr::size_t UniformBlock::size() const {
    return std::accumulate(
        this->bindings.begin(), this->bindings.end(), Pr::size_t{0},
        [](auto sum, const auto& binding) { return sum + binding.size(); });
};

[[nodiscard]] Pr::size_t UniformBinding::size() const {
    return SizeOfType(this->data_type);
};

}  // namespace Pr
