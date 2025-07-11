module presto.assets.mesh;

import std;

import presto.internal.managers.rendering_manager;
import presto.internal;

import presto.assets.image;

import presto.internal.rendering;

import presto.types.core;

namespace Pr {

struct MeshAsset::Impl {
    MeshData mesh_data;

    Pr::Ptr<Pr::MaterialInstance> default_material;

    BoundingBox box;
};

MeshAsset::MeshAsset() { impl_ = std::make_unique<Impl>(); };
MeshAsset::~MeshAsset() = default;

bool MeshAsset::load() {
    this->impl_->mesh_data.pipeline_id = PR_PIPELINE_DEFAULT_3D;

    impl_->registration_id = RenderingManager::get().loadMesh(impl_->mesh_data);
    return impl_->registration_id != PR_UNREGISTERED;
}

MeshAsset& MeshAsset::setDefaultMaterial(
    const Pr::Ptr<Pr::MaterialInstance>& material) {
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
        Pr::Log(
            ERROR,
            "Unable to modify MeshAsset \"{}\" while it is loaded. It must be "
            "unloaded before being modified.\nSkipping this modification.",
            this->name());
        return false;
    }

    return true;
};

Pr::Ptr<Pr::MaterialInstance>& MeshAsset::defaultMaterial() const {
    return impl_->default_material;
};

/*
MeshAsset& MeshAsset::setMeshData(MeshData data) {
    impl_->mesh_data = std::move(data);
    return *this;
};
*/

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
