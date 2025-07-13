module presto.internal.rendering.types;

import std;

import presto.assets.mesh;

namespace Pr {

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

}  // namespace Pr
