#pragma once

#include "../Objects/Component.h"

#include "Rendering/Types/DrawInfo.h"
#include "Rendering/Types/ModelComponent.h"
#include "Rendering/Vulkan/VulkanVertex.h"

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
