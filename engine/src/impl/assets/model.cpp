module presto.assets.model;

import std;

import presto.core.logging;
import presto.assets.image;

import presto.rendering.vertices;

import presto.internal.managers.asset_manager;
import presto.internal.managers.rendering_manager;
import presto.internal.rendering;
import presto.internal.loading;

namespace Pr {
bool ModelAsset::load() { return true; };

ModelAsset::ModelAsset(asset_name_t modelName) : Asset(std::move(modelName)) {}

BoundingBox ModelAsset::getBoundingBox() {
    BoundingBox box{};

    for (const MeshDraw& draw : draws_) {
        // TODO: Fix this to work with the new meshes
        // box.merge(draw.mesh->getBoundingBox());
    }

    return box;
}

ModelAsset& ModelAsset::addMesh(Pr::Ptr<Pr::MeshAsset> mesh,
                                Pr::Ptr<Pr::MaterialInstance> material) {
    Pr::Assert(mesh != nullptr,
               "An added mesh to a model asset must not be null.");

    if (material == nullptr) {
        Pr::CoreLog(
            WARN, "A mesh has been added to a ModelAsset without a material.");
    }

    draws_.push_back(MeshDraw{.mesh{mesh}, .material{material}});

    return *this;
};

ModelAsset& ModelAsset::clear() {
    draws_.clear();

    return *this;
}

Pr::size_t ModelAsset::meshCount() const { return this->draws_.size(); };

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

Pr::Ptr<Pr::ModelAsset> MeshSource::loadModel(Pr::string modelName,
                                              bool allowReload) {
    LoadedModel* model{getLoadedModel(modelName)};
    if (model == nullptr) {
        Pr::CoreLog(
            WARN,
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
    Pr::CoreAssert(
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
        Pr::CoreAssert(new_mesh != nullptr,
                       "Mesh {} could not be loaded from MeshSource.",
                       modelName);
        Ptr<MeshAsset> new_mesh_asset{
            AssetManager::get().newAsset<MeshAsset>()};
        new_mesh_asset->setMesh(new_mesh);

        Pr::Ptr<Pr::MaterialInstance> default_material{nullptr};

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

        model->ptr->addMesh(new_mesh_asset, default_material);
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
        Pr::CoreLog(WARN,
                    "Unable to load {} as it couldn't be found in {}. Skipping "
                    "this "
                    "request.",
                    modelName, this->path().string());
        return;
    }

    this->unloadModel(*model);
}

Pr::Ptr<Pr::MaterialInstance> MeshSource::loadMaterial(Pr::string materialName,
                                                       bool allowReload) {
    LoadedMaterial* material{getLoadedMaterial(materialName)};
    if (material == nullptr) {
        Pr::CoreLog(WARN,
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
    std::dynamic_pointer_cast<MaterialInstanceImpl>(material->ptr)
        ->setFromImport(material->material_import, textures_);
    return material->ptr;
}

MeshSource::MeshSource(AssetPath filepath)
    : AssetSource(std::move(filepath), true) {
    reloadFile();
};

void MeshSource::unload() {
    // TODO: Make this unload all models and materials that belong to the file
};

Pr::Ptr<Pr::ModelAsset> MeshSource::getModel(const Pr::string& name) {
    if (LoadedModel* loaded{getLoadedModel(name)}; loaded != nullptr) {
        return loaded->ptr;
    }

    return nullptr;
};

Pr::Ptr<Pr::MaterialInstance> MeshSource::getMaterial(const Pr::string& name) {
    if (LoadedMaterial* loaded{getLoadedMaterial(name)}; loaded != nullptr) {
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
    Pr::CoreAssert(pipeline_structure != nullptr,
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
    Pr::Assert(imported_data.models.size() > 0,
               std::format("No models could be found in {}.", path().string()));

    // Turn the textures into assets
    textures_.resize(imported_data.textures.size());
    for (Pr::size_t i{0}; i < imported_data.textures.size(); ++i) {
        ImportedTexture& texture{imported_data.textures[i]};
        if (texture.name.empty()) {
            Pr::CoreLog(WARN,
                        "Imported texture ({}x{}) has no name. Skipping this "
                        "import.",
                        texture.image.width, texture.image.height);
            continue;
        }

        // const Pr::Ptr<Pr::ImageAsset>&
        // image_ptr{am.createImageAsset(texture.name, texture.image)};

        // TODO: Make this cache images that it gets from the import rather
        // than create a new asset for each one
        const Pr::Ptr<Pr::ImageAsset>& image_ptr{
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

}  // namespace Pr
