module presto.rendering.vertices;

import presto.internal.rendering;

namespace Pr {

void MeshData::setVertices(const ImportedAttributeList& attributes) {
    auto processed{processVertices<Vertex3D>(attributes)};

    // TODO: Put checks here to make sure the processed vertices are well
    // formed
    this->vertices = std::move(processed);
};

}  // namespace Pr
