#include "Memory/Allocator.h"
#include "Presto/Aliases/Handles.h"
#include "Presto/Rendering/Pipeline.h"

#include "Rendering/MeshRegistrationData.h"

namespace Presto {

struct AllocatedPipeline {
    pipeline_id_t id;
    Allocated<Pipeline> pipeline;
    MaterialPtr default_material{nullptr};
};

using pipeline_allocator_t = Allocator<pipeline_id_t, AllocatedPipeline>;
using mesh_allocator_t =
    Allocator<mesh_registration_id_t, MeshRegistrationData>;

}  // namespace Presto
