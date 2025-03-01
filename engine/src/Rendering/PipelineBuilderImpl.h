#include "Presto/Core/Constants.h"
#include "Presto/Rendering/Pipeline.h"
#include "Presto/Rendering/PipelineBuilder.h"

#include "Memory/AllocatorTypes.h"

namespace Presto {

class Pipeline;

class PipelineBuilderImpl : public PipelineBuilder {
    friend class RenderingManager;

   public:
    [[nodiscard]] pipeline_id_t id() const { return id_; };

    PipelineBuilderImpl& setId(pipeline_id_t id) {
        this->id_ = id;
        return *this;
    };

   protected:
    void addPipeline(Allocated<Pipeline> pipeline) {
        pipelines_->alloc(std::move(pipeline));
    };

   private:
    pipeline_id_t id_{PR_PIPELINE_NONE};

    pipeline_allocator_t* pipelines_;

    void setPipelineList(pipeline_allocator_t& pipelines) {
        pipelines_ = &pipelines;
    };
};

}  // namespace Presto
