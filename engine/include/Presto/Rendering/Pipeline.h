#pragma once

#include "Presto/Rendering/MaterialTypes.h"
#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {

class UniformBuffer;

class Pipeline {
   public:
    virtual void bind() = 0;
    virtual void unbind() = 0;

    [[nodiscard]] virtual const PipelineStructure& getStructure() const {
        return pipelineStructure_;
    };

    virtual void setProperty(std::string property, const void* value) = 0;

    /*virtual void setProperties(std::vector<MaterialProperty>) = 0;*/
    virtual void setProperties(const MaterialStructure&) = 0;

    [[nodiscard]] bool accepts(const MaterialStructure&) const;

    [[nodiscard]] std::vector<PipelineAttribute> getAttributes() const {
        return pipelineStructure_.attributes;
    };

    virtual void setUniformBlock(PR_NUMERIC_ID name, UniformBuffer& buffer) = 0;

    [[nodiscard]] const MaterialStructure& getUniformLayout() const {
        return materialStructure_;
    }

    [[nodiscard]] renderer_pipeline_id_t id() const { return this->id_; }

   protected:
    virtual ~Pipeline() = default;

    void setId(renderer_pipeline_id_t id) { this->id_ = id; }

    MaterialStructure materialStructure_;
    PipelineStructure pipelineStructure_;

   private:
    renderer_pipeline_id_t id_{UNREGISTERED_RENDER_DATA_ID};
};
}  // namespace Presto
