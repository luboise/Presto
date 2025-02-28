#include "Memory/Allocator.h"
#include "Presto/Rendering/Pipeline.h"

#include "Rendering/MeshRegistrationData.h"

namespace Presto {

using pipeline_allocator_t = Allocator<pipeline_id_t, Pipeline>;
using mesh_allocator_t =
    Allocator<mesh_registration_id_t, MeshRegistrationData>;

}  // namespace Presto
