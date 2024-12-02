#pragma once

#include <vulkan/vulkan_core.h>
#include "Presto/Component.h"
#include "Presto/Rendering/Mesh.h"

#include "Transform.h"

namespace Presto {

#define PrestoRenderableConstructorArgs const Mesh &mesh, const Transform &props

    class PRESTO_API Renderable : public Component {
        friend class RenderingManager;

       public:
        [[nodiscard]] const Mesh& getMesh() const;
        const Transform& getProps();

       private:
        Renderable(PrestoRenderableConstructorArgs)
            : _mesh(mesh), _props(props) {}

        const Mesh& _mesh;
        const Transform& _props;
    };

}  // namespace Presto
