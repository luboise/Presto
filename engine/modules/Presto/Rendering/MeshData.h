#pragma once

#include "Presto/Aliases/Handles.h"
#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Constants.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {

struct MeshData {
    pipeline_id_t pipeline_id{PR_PIPELINE_ANY};

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    AnyVertexList vertices;
    IndexList indices;
    // [[nodiscard]] BoundingBox getBoundingBox() const;
    //

    void setVertices(const ImportedAttributeList& attributes);
};

struct MeshDraw {
    const MeshPtr mesh;
    const MaterialPtr material;
};

class Mesh {
    friend class RenderingManager;

   public:
    [[nodiscard]] mesh_registration_id_t registrationId() const;

   private:
    explicit Mesh(mesh_registration_id_t id);
    mesh_registration_id_t registrationId_{PR_UNREGISTERED};
};

}  // namespace Presto
