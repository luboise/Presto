#include "Presto/Rendering/Mesh.h"

namespace Presto {
    Mesh::Mesh(mesh_id_t mesh_id, const VertexList& vertices,
               const IndexList& indices) {
        this->_meshID = mesh_id;
        this->_vertices = vertices;
        this->_indices = indices;
    }

    VertexList Mesh::getVertices() const { return _vertices; }
    IndexList Mesh::getIndices() const { return _indices; }
    uint32_t Mesh::getVertexCount() const { return _vertices.size(); }
}  // namespace Presto
