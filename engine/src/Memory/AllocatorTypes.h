#include "Memory/Allocator.h"
#include "Presto/Rendering/Pipeline.h"

namespace Presto {

using pipeline_allocator_t = Allocator<pipeline_id_t, Pipeline>;

}  // namespace Presto
