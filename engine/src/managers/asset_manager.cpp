module presto.internal.managers.asset_manager;

#define STB_IMAGE_IMPLEMENTATION

import presto.utils;
import :image;

import std;

namespace Pr {

/*
Pr::Ptr<Pr::MaterialInstance> AssetManager::createMaterialFromImport(
const ImportedMaterial& imported_material,
std::vector<Pr::Ptr<Pr::Texture>>& texturePtrs) {
Pr::Ptr<Pr::MaterialInstance> material{RenderingManager::get().createMaterial(
MaterialType::DEFAULT_3D, imported_material.name)};

material->setFromImport(imported_material, texturePtrs);
return material;
};
*/

Pr::Ptr<Pr::MaterialAsset> AssetManager::createMaterialDefinition(
    Pr::string name, const PipelineStructure& structure) {
    if (assets_[AssetType::MATERIAL_DEFINITION].contains(name)) {
        Pr::CoreLog(
            ERROR,
            "Unable to create material definition, as a definition with the "
            "same name already exists.");
        return nullptr;
    }

    Pr::Ptr<Pr::MaterialAsset> new_definition{
        std::make_shared<MaterialAsset>(name, structure)};

    assets_[AssetType::MATERIAL_DEFINITION][name] = new_definition;

    return new_definition;
}

Ptr<MaterialAsset> AssetManager::getMaterialDefinition(pipeline_id_t id) {
    const auto map{assets_[AssetType::MATERIAL_DEFINITION] |
                   std::views::values};

    if (auto it{std::ranges::find_if(
            map,
            [id](const auto& y) -> bool {
                return y->template as<MaterialAsset>()->pipelineId() == id;
            })};
        it != map.end()) {
        return std::dynamic_pointer_cast<MaterialAsset>(it.base()->second);
    }

    return nullptr;
};

}  // namespace Pr
