#pragma once

#include "Presto/Aliases/Handles.h"

#include "Presto/Core/Constants.h"

#include "Presto/Objects/Component.h"
#include "Presto/Objects/Subcomponent.h"

#include "Presto/Assets/ModelAsset.h"  // IWYU pragma: export

namespace Presto {

struct MeshDraw {
    const MeshPtr mesh;
    const MaterialPtr material;

    const MaterialPtr& getMaterial() {
        return material == nullptr ? mesh->defaultMaterial() : material;
    };
};

struct PRESTO_API ModelSubcomponent : public Subcomponent<Component> {
    std::vector<MeshDraw> draws;

    ModelSubcomponent() = delete;

    [[nodiscard]] PR_SIZE meshCount() const { return draws.size(); }

    /*
void setMesh(std::uint8_t index, const MeshPtr& mesh) {
    meshes_[index] = mesh;
}
    */

    /*
void setMaterial(std::uint8_t index, const material_override_t& resource) {
    materialOverrides_[index] = resource;
};
    */
};

}  // namespace Presto
