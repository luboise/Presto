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
        // friend class RenderingManager;

       public:
        Mesh(VertexList, IndexList);

        // [[nodiscard]] mesh_id_t getID() const;
        void setVertices(const VertexList& new_vertices) {
            vertices_ = new_vertices;
        };

        void setVertices(VertexList&& new_vertices) {
            vertices_ = new_vertices;
        };

       private:
        [[nodiscard]] RenderData getRenderData() const override;

        // mesh_id_t _meshID;
        VertexList vertices_;
        IndexList indices_;
    };
}  // namespace Presto
