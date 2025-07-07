module;
#include <glm/glm.hpp>

export module presto.math;

export import :transform_data;
export import :shapes;
export import :quaternion;

export namespace Pr::Math {

auto Radians(auto val) { return glm::radians(val); }
auto Length(auto val) { return glm::length(val); }

}  // namespace Pr::Math
