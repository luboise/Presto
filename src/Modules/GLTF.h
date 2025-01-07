#include "Presto/Rendering/RenderData.h"

namespace Presto {
    using ByteBuffer = std::vector<std::byte>;

    class GLTF {
       public:
        explicit GLTF(const fs::path& filepath);

       private:
        class Accessor {
           public:
            Accessor();
        };

        RenderData getRenderData();

        std::vector<ByteBuffer> buffers_;
        std::vector<Accessor> accessors_;
    };
}  // namespace Presto
