#pragma once

#include "Presto/Component.h"
#include "Presto/Rendering/Vertex.h"

namespace Presto {
#define PrestoMeshConstructorArgs mesh_id_t, const VertexList&, const IndexList&

    using mesh_id_t = id_t;
    class PRESTO_API Mesh : public Component {
        friend class RenderingManager;

       public:
        [[nodiscard]] mesh_id_t getID() const;
        [[nodiscard]] VertexList getVertices() const;
        [[nodiscard]] IndexList getIndices() const;
        [[nodiscard]] uint32_t getVertexCount() const;

       private:
        Mesh(PrestoMeshConstructorArgs);

        mesh_id_t _meshID;
        VertexList _vertices;
        IndexList _indices;
    };
}  // namespace Presto
