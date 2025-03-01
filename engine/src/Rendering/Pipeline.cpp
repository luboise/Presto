#include "Presto/Rendering/Pipeline.h"

namespace Presto {

pipeline_id_t Pipeline::id() const { return pipelineStructure_.pipeline_id; }

void Pipeline::setId(pipeline_id_t id) {
    this->pipelineStructure_.pipeline_id = id;
}

Pipeline::Pipeline(pipeline_id_t id) { setId(id); }

}  // namespace Presto
