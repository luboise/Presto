#pragma once

#include <filesystem>
#include <vector>

#include <glm/glm.hpp>
#include "glm/fwd.hpp"

#include "HandleTypes.h"  // IWYU pragma: export

namespace fs = std::filesystem;

namespace Presto {

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

using mat4 = glm::mat4;

using string = std::string;

using ByteArray = std::vector<std::byte>;

using PR_STRING_ID = Presto::string;

using FilePath = fs::path;

struct VisualExtents {
    std::uint16_t width;
    std::uint16_t height;
};

}  // namespace Presto
