#include <algorithm>
#include "Modules/AssetManager.h"
#include "Modules/RenderingManager.h"

#include "Modules/GLTFLoader.h"
#include "Presto/Assets/AssetSources/MeshSource.h"
#include "Presto/Rendering/MeshData.h"

namespace Presto {

MeshSource::MeshSource(AssetPath filepath)
    : AssetSource(std::move(filepath), true) {
    AssetManager& am{AssetManager::get()};
    RenderingManager& rm{RenderingManager::get()};

    // The 3D pipeline must be defined in order for 3D models to be used.
    const PipelineStructure* pipeline_structure{
        rm.getPipelineStructure(PR_PIPELINE_DEFAULT_3D)};
    PR_CORE_ASSERT(pipeline_structure != nullptr,
                   "The pipeline structure of the default pipelines must "
                   "not be nullptr.");

    std::string filename{path().basename()};
    fs::path file_extension{path().fileExtension()};

    ImportedModelData imported_data;

    if (file_extension == ".gltf" || file_extension == ".glb") {
        GLTFLoader loader;
        importData_ = std::make_unique<ImportedModelData>(loader.load(path()));
    }

    // Get the import
    PR_ASSERT(importData_->models.size() > 0,
              std::format("No models could be found in {}.", path().string()));

    materials_.resize(importData_->materials.size());
    textures_.resize(importData_->textures.size());

    // Turn the textures into assets
    for (std::size_t i{0}; i < importData_->textures.size(); ++i) {
        ImportedTexture& texture{importData_->textures[i]};
        if (texture.name.empty()) {
            PR_WARN(
                "Imported texture ({}x{}) has no name. Skipping this "
                "import.",
                texture.image.width, texture.image.height);
            continue;
        }

        // TODO: Make this cache images that it gets from the import rather
        // than create a new asset for each one
        const auto& image_ptr{am.createImageAsset(texture.name, texture.image)};

        textures_.push_back(rm.createTexture2D(image_ptr));
    }

    // Turn the materials into assets
    for (std::size_t i{0}; i < materials_.size(); ++i) {
        materials_[i] =
            am.createMaterialFromImport(imported_data.materials[i], textures_);
    }

    // Turns the models into model assets (and subsequently their underlying
    // meshes)
    for (size_t i{0}; i < imported_data.models.size(); ++i) {
        const auto& imported_model = imported_data.models[i];

        PR_STRING_ID new_name{imported_model.name};

        ModelPtr model{std::make_shared<ModelAsset>(new_name)};
        for (const ImportedMesh& imported_mesh : imported_model.meshes) {
            MeshPtr mesh{std::make_shared<MeshAsset>()};

            MeshData data{
                .draw_mode = imported_mesh.draw_mode,
                .vertices{},
                .indices = imported_mesh.indices,
            };
            data.setVertices(imported_mesh.attributes);

            mesh->setMeshData(std::move(data));

            // Make sure the material is loaded
            if (imported_mesh.hasMaterial() &&
                materials_[imported_mesh.material_index] != nullptr) {
                mesh->setDefaultMaterial(
                    materials_[imported_mesh.material_index]);
            }

            model->addMesh(mesh);
        }

        am.addAsset(model);
        models_.emplace_back(std::move(model));
    }
};

void MeshSource::load() {
    std::ranges::for_each(
        models_, [](Ptr<ModelAsset>& model) { model->ensureLoaded(); });
    std::ranges::for_each(textures_, [](auto& texture) { texture->load(); });
}

ModelPtr MeshSource::getModel(const Presto::string& name) {
    auto found{std::ranges::find_if(models_, [&name](const ModelPtr& model) {
        return model->name() == name;
    })};

    return found == models_.end() ? nullptr : *found;
};

}  // namespace Presto
