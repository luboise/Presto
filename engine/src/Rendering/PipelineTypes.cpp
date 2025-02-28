#include "Presto/Rendering/PipelineTypes.h"

#include <numeric>
#include "Presto/Rendering/UniformTypes.h"

namespace Presto {

Presto::size_t PipelineUniformBlock::size() const {
    return std::accumulate(
        this->uniforms.begin(), this->uniforms.end(), Presto::size_t{0},
        [](auto sum, const auto& uniform) { return sum + uniform.size(); });
};

Presto::size_t PipelineUniform::size() const { return SizeOfType(this->type); };

Presto::size_t PipelineAttribute::size() const {
    return attributeTypeDetailsOf(this->type).size();
};

Presto::size_t PipelineStructure::stride() const {
    return std::accumulate(this->attributes.begin(), this->attributes.end(),
                           Presto::size_t{0},
                           [](auto sum, const PipelineAttribute& attrib) {
                               return sum + attrib.size();
                           });
};

}  // namespace Presto
