#pragma once

#include "Presto/Components/Renderable.h"
#include "PrestoCore/Rendering/RenderData.h"

namespace Presto {
    // #define PrestoMeshConstructorArgs mesh_id_t, const VertexList&, const
    // IndexList&

    struct MeshCreationInfo {
        VertexList vertices;
        IndexList indices;
    };

    using mesh_id_t = id_t;
    class PRESTO_API Mesh : public Renderable {
        friend class RenderingManager;

       public:
        // [[nodiscard]] mesh_id_t getID() const;
        void setVertices(const VertexList& new_vertices) {
            _vertices = new_vertices;
        };

        void setVertices(VertexList&& new_vertices) {
            _vertices = new_vertices;
        };

       private:
        // Mesh(PrestoMeshConstructorArgs);

        [[nodiscard]] RenderData getRenderData() const override;

        // mesh_id_t _meshID;
        VertexList _vertices;
        IndexList _indices;
    };
}  // namespace Presto
