#include "Presto/Modules/ObjectsModule/Component/Component.h"

#include "Presto/Modules/RenderingModule/Vulkan/VulkanVertex.h"

namespace Presto {
    class PRESTO_API Polygon2D : public Component {
       public:
        Polygon2D(VertexList vertices);

        std::vector<VulkanVertex> vertices;

        VertexList getVertices() const;
        uint32_t getVertexCount() const;

       private:
        VertexList _vertices;
        ModelComponent* _model;
    };
}  // namespace Presto