#pragma once

#include "presto/core/constants.h"
#include "presto/handles/asset_handles.h"

#include "internal/utils.h"

namespace Pr {

struct Buffer;
class Pipeline;

enum class ShaderStage { VERTEX, FRAGMENT };

struct MeshDraw {
    const MeshPtr mesh;
    const MaterialPtr material;
};

struct AllocatedPipeline {
    pipeline_id_t id;
    Allocated<Pipeline> pipeline;
    MaterialPtr default_material{nullptr};
};

enum class MeshDrawMode : Pr::uint8_t {
    POINTS,
    LINES,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP
};

struct MeshRegistrationData {
    // The ID of the mesh's place in the RenderingManager
    mesh_registration_id_t render_manager_id;
    Allocated<Buffer> vertices;
    Allocated<Buffer> indices;

    Pr::size_t index_draw_count{0};

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    // The ID that points to the internal details in the renderer
    mesh_context_id_t context_id{-1U};
};

using pipeline_allocator_t = Allocator<pipeline_id_t, AllocatedPipeline>;
using mesh_allocator_t =
    Allocator<mesh_registration_id_t, MeshRegistrationData>;

}  // namespace Pr
