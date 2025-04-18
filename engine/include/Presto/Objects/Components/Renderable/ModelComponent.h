#pragma once

#include "Presto/Aliases/Handles.h"

#include "Presto/Core/Constants.h"
#include "Presto/Objects/Component.h"

#include "Presto/Assets/ModelAsset.h"  // IWYU pragma: export

namespace Presto {
struct RenderGroup;

using mesh_registration_id_t = PR_NUMERIC_ID;

class PRESTO_API ModelComponent : public Component {
    // friend class RenderingManager;
    friend class EntityManager;
    friend class EntityManagerImpl;

    // TODO: Make the render trigger when the entity enters the scene so
    // that this friend declaration isn't necessary
    friend class RenderingManager;

    using material_override_t = MaterialPtr;

   public:
    ModelComponent() = delete;

    MeshPtr getMesh(std::uint8_t index = 0) {
        if (meshes_.size() == 0 || index >= meshes_.size()) {
            return nullptr;
        }
        return meshes_[index];
    }

    [[nodiscard]] const std::vector<MeshPtr>& getMeshes() const {
        return meshes_;
    }

    void setMesh(std::uint8_t index, const MeshPtr& mesh) {
        meshes_[index] = mesh;
    }

    [[nodiscard]] PR_SIZE meshCount() const { return meshes_.size(); }

    material_override_t& getMaterial(std::size_t index) {
        if (materialOverrides_[index] == nullptr) {
            return this->meshes_[index]->defaultMaterial();
        }

        return materialOverrides_[index];
    };

    void setMaterial(std::uint8_t index, const material_override_t& resource) {
        materialOverrides_[index] = resource;
    };

   private:
    void onEnterScene() override;

    explicit ModelComponent(const ModelPtr&);

    std::vector<MeshPtr> meshes_;
    std::vector<material_override_t> materialOverrides_;
};
}  // namespace Presto
