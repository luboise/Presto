#include "Presto/Rendering/Pipeline.h"
#include <algorithm>

namespace Presto {

bool Pipeline::accepts(const MaterialStructure& inStructure) const {
    return std::ranges::all_of(
        structure_.properties,
        [&inStructure](const auto& out_property) -> bool {
            const MaterialProperty* in_property{
            inStructure.getProperty(out_property.name)};

            return in_property != nullptr &&
                   in_property->compatibleWith(out_property);
        });

    return true;
};

}  // namespace Presto
