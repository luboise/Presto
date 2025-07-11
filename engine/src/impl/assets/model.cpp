module presto.assets.model;

import std;

import presto.core.logging;

import presto.internal.managers.asset_manager;

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
    material->ptr->setFromImport(material->material_import, textures_);
    return material->ptr;
}

}  // namespace Pr
