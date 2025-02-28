#pragma once

#include <utility>

#include "Presto/Rendering/MaterialTypes.h"
#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Rendering/UniformTypes.h"

namespace Presto {

class UniformBuffer;

class Pipeline {
   public:
    virtual void bind() = 0;
    virtual void unbind() = 0;

    [[nodiscard]] virtual const PipelineStructure& getStructure() const {
        return pipelineStructure_;
    };

    virtual uniform_index_t getIndex(uniform_name_t name) = 0;

    template <typename T>
    void setUniform(uniform_name_t name, T data) {
        uniform_index_t index{getIndex(std::move(name))};
        if (index == PR_INVALID_UNIFORM) {
            PR_ERROR(
                "Unable to set uniform \"{}\" (its index could not be found)",
                name);

            return;
        }
        setUniform(index, data);
    }

    /**
     *  Sets the value of a uniform variable in the pipeline. For OpenGL, these
     * is expected to be used for regular uniform variables.
     */
    virtual void setUniform(uniform_index_t index, Presto::uint32_t value) = 0;

    virtual void setUniform(uniform_index_t index, Presto::int32_t value) = 0;
    virtual void setUniform(uniform_index_t index, Presto::float32_t value) = 0;

    virtual void setUniform(uniform_index_t index, Presto::vec2 value) = 0;
    virtual void setUniform(uniform_index_t index, Presto::vec3 value) = 0;
    virtual void setUniform(uniform_index_t index, Presto::vec4 value) = 0;
    virtual void setUniform(uniform_index_t index, Presto::mat4 value) = 0;

    // [[nodiscard]] bool accepts(const UniformLayout&) const;

    [[nodiscard]] const std::vector<PipelineAttribute>& getAttributes() const {
        return pipelineStructure_.attributes;
    };

    virtual void setUniformBlock(uniform_index_t index,
                                 UniformBuffer& buffer) = 0;

    [[nodiscard]] renderer_pipeline_id_t id() const { return this->id_; }

    virtual ~Pipeline() = default;

   protected:
    void setId(renderer_pipeline_id_t id) { this->id_ = id; }

    PipelineStructure pipelineStructure_;

   private:
    pipeline_id_t id_{PR_ANY_PIPELINE};
};
}  // namespace Presto
