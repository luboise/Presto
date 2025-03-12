#pragma once

namespace Presto {

struct Line {
    Presto::vec3 from{0, 0, 0};
    Presto::vec3 to{1, 1, 1};
    Presto::vec4 colour{1, 1, 1, 1};
};

}  // namespace Presto
