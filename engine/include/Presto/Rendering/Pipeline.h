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
    // virtual void setProperties(const UniformLayout&) = 0;

    // [[nodiscard]] bool accepts(const UniformLayout&) const;

    [[nodiscard]] const std::vector<PipelineAttribute>& getAttributes() const {
        return pipelineStructure_.attributes;
    };

    virtual void setUniformBlock(PR_NUMERIC_ID name, UniformBuffer& buffer) = 0;

    [[nodiscard]] renderer_pipeline_id_t id() const { return this->id_; }

    virtual ~Pipeline() = default;

   protected:
    void setId(renderer_pipeline_id_t id) { this->id_ = id; }

    PipelineStructure pipelineStructure_;

   private:
    pipeline_id_t id_{PR_ANY_PIPELINE};
};
}  // namespace Presto
