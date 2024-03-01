#pragma once

#include "Presto/Modules/ObjectsModule/Component/Component.h"

#include "Presto/Modules/RenderingModule/Types/ModelComponent.h"
#include "Presto/Modules/RenderingModule/Vulkan/VulkanVertex.h"

namespace Presto {
    class PRESTO_API Polygon2D : public Component {
       public:
        Polygon2D(const VertexList& vertices, const IndexList& indices);

        VertexList getVertices() const;
        IndexList getIndices() const;
        uint32_t getVertexCount() const;

       private:
        VertexList _vertices;
        IndexList _indices;
        ModelComponent* _model;
    };
}  // namespace Presto