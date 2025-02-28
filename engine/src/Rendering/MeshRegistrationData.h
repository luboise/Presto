#pragma once
#include "Presto/Rendering/RenderTypes.h"
namespace Presto {
class Buffer;

struct MeshRegistrationData {
    // The ID of the mesh's place in the RenderingManager
    mesh_registration_id_t render_manager_id;
    Allocated<Buffer> vertices;
    Allocated<Buffer> indices;

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    // The ID that points to the internal details in the renderer
    mesh_context_id_t context_id{-1U};
};

}  // namespace Presto
