module;

// #include <glm/gtc/type_precision.hpp>

#include <glm/detail/type_float.hpp>
#include <glm/detail/type_mat3x4.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

export module presto.types.core:base;

import std;

export namespace Pr {

namespace fs = std::filesystem;
using ByteArray = std::vector<std::byte>;

using int16_t = std::int16_t;
using uint16_t = std::uint16_t;

using float32_t = glm::float32_t;
using float64_t = glm::float64_t;

using size_t = std::size_t;

using int8_t = std::int8_t;
using int32_t = std::int32_t;

using uint8_t = std::uint8_t;
using uint16_t = std::uint16_t;
using uint32_t = std::uint32_t;

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

using mat3 = glm::mat3;
using mat4 = glm::mat4;

using string = std::string;

using FilePath = fs::path;

using PR_NUMERIC_ID = Pr::uint32_t;

}  // namespace Pr
