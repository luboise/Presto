#include "Presto/Rendering/UniformTypes.h"

namespace Presto {

[[nodiscard]] Presto::size_t UniformBlock::size() const {
    return std::accumulate(
        this->bindings.begin(), this->bindings.end(), Presto::size_t{0},
        [](auto sum, const auto& binding) { return sum + binding.size(); });
};

[[nodiscard]] Presto::size_t UniformBinding::size() const {
    return SizeOfType(this->type);
};

}  // namespace Presto
