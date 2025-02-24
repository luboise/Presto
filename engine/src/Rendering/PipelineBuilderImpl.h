#include "Presto/Rendering/Pipeline.h"
#include "Presto/Rendering/PipelineBuilder.h"

#include "Memory/AllocatorTypes.h"

namespace Presto {

class Pipeline;

class PipelineBuilderImpl : public PipelineBuilder {
    friend class RenderingManager;

   protected:
    void addPipeline(Allocated<Pipeline> pipeline) {
        pipelines_->alloc(std::move(pipeline));
    };

   private:
    pipeline_allocator_t* pipelines_;

    void setPipelineList(pipeline_allocator_t& pipelines) {
        pipelines_ = &pipelines;
    };
};
}  // namespace Presto
