#include "Presto/Rendering/Polygon2D.h"

namespace Presto {
    Polygon2D::Polygon2D(const VertexList& vertices, const IndexList& indices) {
        this->_vertices = vertices;
        this->_indices = indices;
    }

    VertexList Polygon2D::getVertices() const { return _vertices; }
    IndexList Polygon2D::getIndices() const { return _indices; }
    uint32_t Polygon2D::getVertexCount() const { return _vertices.size(); }
}  // namespace Presto
