#include "Polygon2D.h"

namespace Presto {
    VertexList Polygon2D::getVertices() const { return _vertices; }
    uint32_t Polygon2D::getVertexCount() const { return _vertices.size(); }
}  // namespace Presto
