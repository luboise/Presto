#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {
    enum class MaterialType {
        DEFAULT_3D,
    };

    struct MaterialData {
        renderer_material_id_t materialType{PR_MATERIAL_DEFAULT_3D};
        renderer_texture_id_t diffuseTexture{UNREGISTERED_RENDER_DATA_ID};

        // TODO: Implement lighting props
        double lightingProp1;
        double lightingProp2;
        double lightingProp3;

        void* data;

        // TODO: Implement custom shaders
    };
}  // namespace Presto
