export module presto.math:types;

import glm;

export namespace Pr::Math {

auto Radians(auto val) { return glm::radians(val); }
auto Length(auto val) { return glm::length(val); }
auto Normalise(auto val) { return glm::normalize(val); }
auto Dot(auto val, auto val2) { return glm::dot(val, val2); }
auto Cross(auto val) { return glm::cross(val); }
auto Cos(auto val) { return glm::cos(val); }
auto Sin(auto val) { return glm::sin(val); }

}  // namespace Pr::Math
