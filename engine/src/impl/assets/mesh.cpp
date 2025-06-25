module presto.assets.mesh;

#include <utility>

import presto.internal.managers.rendering;
import presto.internal;

import presto.core.types;

export namespace Pr {

struct MeshAsset::Impl {
    MeshData mesh_data;

    MaterialPtr default_material;

    BoundingBox box;
};

MeshAsset::MeshAsset() { impl_ = std::make_unique<Impl>(); };
MeshAsset::~MeshAsset() = default;

bool MeshAsset::load() {
    this->impl_->mesh_data.pipeline_id = PR_PIPELINE_DEFAULT_3D;

    impl_->registration_id = RenderingManager::get().loadMesh(impl_->mesh_data);
    return impl_->registration_id != PR_UNREGISTERED;
}

MeshAsset& MeshAsset::setDefaultMaterial(const MaterialPtr& material) {
    if (!modifiable()) {
        return *this;
    }

    impl_->default_material = material;
    return *this;
};
//
// TODO: Implement bounding box logic on import of mesh
BoundingBox MeshAsset::getBoundingBox() const { return impl_->box; }

bool MeshAsset::modifiable() const {
    if (this->loaded()) {
        PR_ERROR(
            "Unable to modify MeshAsset \"{}\" while it is loaded. It must be "
            "unloaded before being modified.\nSkipping this modification.",
            this->name());
        return false;
    }

    return true;
};

MaterialPtr& MeshAsset::defaultMaterial() const {
    return impl_->default_material;
};

MeshAsset& MeshAsset::setMeshData(MeshData data) {
    impl_->mesh_data = std::move(data);
    return *this;
};

MeshSource::MeshSource(AssetPath filepath)
    : AssetSource(std::move(filepath), true) {
    reloadFile();
};

void MeshSource::unload() {
    // TODO: Make this unload all models and materials that belong to the file
};

ModelPtr MeshSource::getModel(const Pr::string& name) {
    if (LoadedModel * loaded{getLoadedModel(name)}; loaded != nullptr) {
        return loaded->ptr;
    }

    return nullptr;
};

MaterialPtr MeshSource::getMaterial(const Pr::string& name) {
    if (LoadedMaterial * loaded{getLoadedMaterial(name)}; loaded != nullptr) {
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

    Pr::string filename{path().basename()};
    fs::path file_extension{path().fileExtension()};

    ImportedModelData imported_data;

    if (file_extension == ".gltf" || file_extension == ".glb") {
        GLTFLoader loader;
        imported_data = loader.load(path());
    }

    // Get the import
    PR_ASSERT(imported_data.models.size() > 0,
              std::format("No models could be found in {}.", path().string()));

    // Turn the textures into assets
    textures_.resize(imported_data.textures.size());
    for (Pr::size_t i{0}; i < imported_data.textures.size(); ++i) {
        ImportedTexture& texture{imported_data.textures[i]};
        if (texture.name.empty()) {
            PR_WARN(
                "Imported texture ({}x{}) has no name. Skipping this "
                "import.",
                texture.image.width, texture.image.height);
            continue;
        }

        // const ImagePtr& image_ptr{am.createImageAsset(texture.name,
        // texture.image)};

        // TODO: Make this cache images that it gets from the import rather
        // than create a new asset for each one
        const ImagePtr& image_ptr{
            am.newAsset<ImageAsset>(texture.name, texture.image)};

        textures_[i] = rm.createTexture2D(image_ptr);
    }

    // Store the material imports
    materials_.resize(imported_data.materials.size());
    for (Pr::size_t i{0}; i < materials_.size(); ++i) {
        ImportedMaterial& mat{imported_data.materials[i]};

        materials_[i] = {.name{mat.name}, .material_import{std::move(mat)}};
    }

    // For each model in the import
    for (size_t i{0}; i < imported_data.models.size(); ++i) {
        const auto& imported_model = imported_data.models[i];

        // See if the model was previously loaded, so we can reuse the
        // LoadedModel structure
        LoadedModel* loaded_model{getLoadedModel(imported_model.name)};

        // If the model was previously loaded, clear its mesh imports and unload
        // it
        if (loaded_model != nullptr) {
            loaded_model->mesh_imports.clear();
            unloadModel(*loaded_model);
        } else {
            // Otherwise, create a new LoadedModel for this new model
            LoadedModel& new_model{models_.emplace_back(LoadedModel{
                .name{imported_model.name}, .mesh_imports{}, .meshes{}})};
            loaded_model = &new_model;
        }

        // Put the meshes into the LoadedModel structure
        for (const ImportedMesh& imported_mesh : imported_model.meshes) {
            if (imported_mesh.hasMaterial()) {
                auto index{imported_mesh.material_index};
                if (materials_[index].ptr == nullptr) {
                    // TODO: Make this import directly instead of doing the name
                    // search
                    loadMaterial(materials_[index].name);
                }
            };
            loaded_model->mesh_imports.push_back(imported_mesh);
        }
    }
};

MeshSource::~MeshSource() {
    if (loaded()) {
        unload();
    }
};

MeshSource::LoadedModel* MeshSource::getLoadedModel(const Pr::string& name) {
    auto found{std::ranges::find_if(models_, [&name](const LoadedModel& model) {
        return model.name == name;
    })};
    return found == models_.end() ? nullptr : found.base();
};

MeshSource::LoadedMaterial* MeshSource::getLoadedMaterial(
    const Pr::string& name) {
    auto found{std::ranges::find_if(materials_,
                                    [&name](const LoadedMaterial& material) {
                                        return material.name == name;
                                    })};
    return found == materials_.end() ? nullptr : found.base();
};

ModelPtr MeshSource::loadModel(Pr::string modelName, bool allowReload) {
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

        MaterialPtr default_material{nullptr};

        // If the mesh has a material, try to load it
        if (imported_mesh.hasMaterial()) {
            if (!materials_.empty() &&
                imported_mesh.material_index <= materials_.size()) {
                LoadedMaterial& loaded_material{
                    materials_[imported_mesh.material_index]};

                // TODO: Make this not do the lookup and just load it on the
                // spot if performance is bad
                default_material = loadMaterial(loaded_material.name);
            }
        }

        model->ptr->addMesh(new_mesh, default_material);
    }

    return model->ptr;
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

void MeshSource::unloadModel(const Pr::string& modelName) {
    LoadedModel* model{getLoadedModel(modelName)};

    if (model == nullptr) {
        PR_CORE_WARN(
            "Unable to load {} as it couldn't be found in {}. Skipping "
            "this "
            "request.",
            modelName, this->path().string());
        return;
    }

    this->unloadModel(*model);
}

MaterialPtr MeshSource::loadMaterial(Pr::string materialName,
                                     bool allowReload) {
    LoadedMaterial* material{getLoadedMaterial(materialName)};
    if (material == nullptr) {
        PR_CORE_WARN(
            "Unable to load {} as it couldn't be found in {}. Skipping "
            "this "
            "request.",
            materialName, this->path().string());
        return nullptr;
    }

    if (material->ptr != nullptr) {
        if (!allowReload) {
            return nullptr;
        }

        // TODO: Add unloading here
    } else {
        material->ptr = RenderingManager::get().createMaterial(
            MaterialType::DEFAULT_3D, material->name);
    }

    // Update the existing material pointer with the new values from the
    // file
    material->ptr->setFromImport(material->material_import, textures_);
    return material->ptr;
}

void BoundingBox::merge(const BoundingBox& other) {
    this->x_min = std::min(x_min, other.x_min);
    this->y_min = std::min(y_min, other.y_min);
    this->z_min = std::min(z_min, other.z_min);

    this->x_max = std::min(x_max, other.x_max);
    this->y_max = std::min(y_max, other.y_max);
    this->z_max = std::min(z_max, other.z_max);
}

double BoundingBox::getNormalisingFactor() const {
    double new_val{0};

    double max{x_min};

    new_val = abs(x_max);
    max = std::max(new_val, max);

    new_val = abs(y_min);
    max = std::max(new_val, max);

    new_val = abs(y_max);
    max = std::max(new_val, max);

    new_val = abs(z_min);
    max = std::max(new_val, max);

    new_val = abs(z_max);
    max = std::max(new_val, max);

    if (max == 0) {
        return 0;
    }

    return 0.5 / max;
}

}  // namespace Pr
