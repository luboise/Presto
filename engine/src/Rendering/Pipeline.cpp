#include "Presto/Rendering/Pipeline.h"
#include <algorithm>
#include <utility>

namespace Presto {

bool Pipeline::accepts(const MaterialStructure& inStructure) const {
    return std::ranges::all_of(
        materialStructure_.properties,
        [&inStructure](const auto& out_property) -> bool {
            const MaterialProperty* in_property{
                inStructure.getProperty(out_property.name)};

            return in_property != nullptr &&
                   in_property->compatibleWith(out_property);
        });

    return true;
};

Pipeline::Pipeline(renderer_pipeline_id_t id, PipelineStructure structure)
    : pipelineStructure_(std::move(structure)), id_(id) {};

}  // namespace Presto
