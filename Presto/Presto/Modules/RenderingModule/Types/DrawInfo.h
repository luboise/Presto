#pragma once

namespace Presto {
    typedef std::vector<uint32_t> IndexList;

    struct PRESTO_API DrawInfo {
        size_t vertexCount = 0;
        size_t indexCount = 0;
        uint32_t vbOffset = 0;
        int32_t ibOffset = 0;
    };
}  // namespace Presto
