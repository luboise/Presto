#pragma once

#include "Presto/Assets/ImportTypes.h"
#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {

class Pipeline {
   public:
    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void setProperty(std::string property, const void* value) = 0;

    /*virtual void setProperties(std::vector<MaterialProperty>) = 0;*/
    virtual void setProperties(const MaterialStructure&) = 0;

    [[nodiscard]] bool accepts(const MaterialStructure&) const;

    [[nodiscard]] std::vector<PipelineAttribute> getAttributes() const {
        return attributes_;
    };

    [[nodiscard]] const MaterialStructure& getUniformLayout() const {
        return structure_;
    }

   protected:
    virtual ~Pipeline() = default;

    MaterialStructure structure_;
    std::vector<PipelineAttribute> attributes_;
    std::vector<PipelineUniform> uniforms_;

    void setId();

   private:
    renderer_pipeline_id_t id_{UNREGISTERED_RENDER_DATA_ID};
};
}  // namespace Presto
