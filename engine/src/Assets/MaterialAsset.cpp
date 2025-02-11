#include <algorithm>
#include <utility>

#include "Presto/Assets/MaterialAsset.h"

#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Constants.h"
#include "Presto/Modules/RenderingManager.h"
#include "Presto/Rendering/PipelineTypes.h"

namespace Presto {
MaterialAsset::MaterialAsset(PR_STRING_ID name, renderer_pipeline_id_t id)
    : Asset(std::move(name)), pipelineId_(id) {
    PipelineStructure* structure {
        RenderingManager::get().getPipelineStructure(id);
    };

    PR_ASSERT(structure != nullptr,
              std::format("A material must be made from an existing pipeline, "
                          "and a pipeline with id {} does not exist.",
                          id));

    this->properties_ = structure->pipeline_id;
}

ImagePtr MaterialAsset::getImage() const { return diffuseImage_; }

MaterialStructure MaterialAsset::getStructure() const {
    MaterialStructure data{};

    data.materialType = PR_PIPELINE_DEFAULT_3D;

    data.properties = properties_;

    return data;
}

void MaterialAsset::load() {
    RenderingManager::get().loadImageOnGpu(this->diffuseImage_);
};

MaterialProperty* MaterialAsset::getProperty(const PR_STRING_ID& name) {
    if (const auto found{
            std::ranges::find_if(properties_,
                                 [name](const MaterialProperty& prop) {
                                     return prop.name == name;
                                 })};
        found != properties_.end()) {
        return &*found;
    }

    return nullptr;
}

/*bool MaterialAsset::operator==(renderer_pipeline_id_t id) const {*/
/*    return pipelineId_ == id;*/
/*}*/

MaterialInstancePtr MaterialAsset::createInstance() const {

};

}  // namespace Presto
