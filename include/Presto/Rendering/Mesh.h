#pragma once

#include "Presto/Component.h"
#include "Presto/Rendering/Vertex.h"

namespace Presto {
#define PrestoMeshConstructorArgs mesh_id_t, const VertexList&, const IndexList&

    typedef id_t mesh_id_t;
    class PRESTO_API Mesh : public Component {
        friend class RenderingManager;

       public:
        mesh_id_t getID() const;
        VertexList getVertices() const;
        IndexList getIndices() const;
        uint32_t getVertexCount() const;

       private:
        Mesh(PrestoMeshConstructorArgs);

        mesh_id_t _meshID;
        VertexList _vertices;
        IndexList _indices;
    };
}  // namespace Presto
