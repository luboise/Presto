#pragma once

#include <vulkan/vulkan_core.h>
#include "PrestoCore/Objects/Component.h"
#include "PrestoCore/Rendering/Mesh.h"

#include "RenderableProps.h"

namespace Presto {

#define PrestoRenderableConstructorArgs \
    const Mesh &mesh, const RenderableProps &props

    class PRESTO_API Renderable : public Component {
        friend class RenderingManager;

       public:
        const Mesh& getMesh() const;
        const RenderableProps& getProps();

       private:
        Renderable(PrestoRenderableConstructorArgs)
            : _mesh(mesh), _props(props) {}

        const Mesh& _mesh;
        const RenderableProps& _props;
    };

}  // namespace Presto
