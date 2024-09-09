#pragma once

namespace Presto {
    struct ShaderMatrices {
        glm::mat4 modelView;
        glm::mat4 projection;
    };

    struct DrawInfo {
        size_t vertexCount = 0;
        size_t indexCount = 0;
        uint32_t vbOffset = 0;
        int32_t ibOffset = 0;
    };

}  // namespace Presto
