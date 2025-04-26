#include <algorithm>
#include "Modules/AssetManager.h"
#include "Modules/RenderingManager.h"

#include "Modules/GLTFLoader.h"
#include "Presto/Assets/AssetSources/MeshSource.h"
#include "Presto/Rendering/MeshData.h"

namespace Presto {

MeshSource::MeshSource(AssetPath filepath)
    : AssetSource(std::move(filepath), true) {
    reloadFile();
};

ModelPtr MeshSource::loadModel(Presto::string modelName, bool allowReload) {
    LoadedModel* model{getLoadedModel(modelName)};
    if (model == nullptr) {
        PR_CORE_WARN(
            "Unable to load {} as it couldn't be found in {}. Skipping this "
            "request.",
            modelName, this->path().string());
        return nullptr;
    }

    // If the model has been loaded previously
    if (model->ptr != nullptr) {
        if (!allowReload) {
            return model->ptr;
        }

        unloadModel(*model);

    } else {
        Ptr<ModelAsset> new_ptr{
            AssetManager::get().newAsset<ModelAsset>(modelName)};

        model->ptr = std::move(new_ptr);
    }

    // Past this point, it is assumed that the ModelAsset is empty
    PR_CORE_ASSERT(
        model->ptr->meshCount() == 0,
        "A model asset that was just cleared must have 0 meshes in it.");

    for (ImportedMesh& imported_mesh : model->mesh_imports) {
        MeshData data{
            .draw_mode = imported_mesh.draw_mode,
            .vertices{},
            .indices = imported_mesh.indices,
        };
        data.setVertices(imported_mesh.attributes);

        Ptr<Mesh> new_mesh{RenderingManager::get().loadMesh(data)};

        // Make sure the material is loaded
        if (imported_mesh.hasMaterial() &&
            materials_[imported_mesh.material_index] != nullptr) {
            mesh->setDefaultMaterial(materials_[imported_mesh.material_index]);
        }

        model->ptr->addMesh(new_mesh);
    }
};

void MeshSource::unloadModel(LoadedModel& loadedModel) {
    // If the model has been loaded previously
    if (loadedModel.ptr != nullptr) {
        // Remove the meshes from the ModelAsset
        loadedModel.ptr->clear();
    }

    // Destroy the meshes in the renderer
    for (Ptr<Mesh>& mesh : loadedModel.meshes) {
        RenderingManager::get().unloadMesh(std::move(mesh));
    }
    loadedModel.meshes.clear();
};

void MeshSource::unloadModel(const Presto::string& modelName) {
    LoadedModel* model{getLoadedModel(modelName)};

    if (model == nullptr) {
        PR_CORE_WARN(
            "Unable to load {} as it couldn't be found in {}. Skipping this "
            "request.",
            modelName, this->path().string());
        return;
    }

    this->unloadModel(*model);
}

/*
void MeshSource::load() {
    std::ranges::for_each(
        models_, [](Ptr<ModelAsset>& model) { model->ensureLoaded(); });
    std::ranges::for_each(textures_, [](auto& texture) { texture->load(); });
}
*/

ModelPtr MeshSource::getModel(const Presto::string& name) {
    if (auto* loaded{getLoadedModel(name)}; loaded != nullptr) {
        return loaded->ptr;
    }

    return nullptr;
};

void MeshSource::reloadFile() {
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

/*
void MeshSource::reloadFile() {
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
*/

MeshSource::~MeshSource() {};

MeshSource::LoadedModel* MeshSource::getLoadedModel(
    const Presto::string& name) {
    auto found{std::ranges::find_if(models_, [&name](const LoadedModel& model) {
        return model.name == name;
    })};
    return found == models_.end() ? nullptr : found.base();
};

}  // namespace Presto
