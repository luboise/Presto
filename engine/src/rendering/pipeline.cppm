export module presto.internal.rendering:pipeline;

import presto.core;

export namespace Presto {

struct PipelineAttribute {
    // Index of the attribute
    PR_NUMERIC_ID layout;

    ShaderDataType type;
    Presto::string name;

    Presto::size_t offset;

    [[nodiscard]] Presto::size_t size() const;
};

struct PipelineUniform {
    PR_NUMERIC_ID location;
    UniformVariableType data_type;
    Presto::string name;

    Presto::size_t offset;

    [[nodiscard]] Presto::size_t size() const;
};

struct PipelineUniformBlock {
    Presto::uint8_t binding;
    Presto::string name;
    std::vector<PipelineUniform> uniforms;

    [[nodiscard]] Presto::size_t size() const;
};

struct PipelineStructure {
    pipeline_id_t pipeline_id;

    bool uses_global_uniforms;
    bool uses_object_uniforms;
    bool uses_alpha_channel;

    std::vector<PipelineAttribute> attributes;
    std::vector<PipelineUniform> uniforms;
    std::vector<PipelineUniformBlock> uniform_blocks;

    [[nodiscard]] Presto::size_t stride() const;
};

class Pipeline {
    friend class PipelineBuilder;

   public:
    virtual void bind() = 0;
    virtual void unbind() = 0;

    [[nodiscard]] virtual const PipelineStructure& getStructure() const {
        return pipelineStructure_;
    };

    virtual uniform_index_t getIndex(uniform_name_t name) = 0;

    template <typename T>
    void setUniform(uniform_name_t name, T data) {
        uniform_index_t index{getIndex(name)};
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

    virtual void setUniformBlock(uniform_index_t index,
                                 UniformBuffer& buffer) = 0;

    [[nodiscard]] pipeline_id_t id() const;

    virtual ~Pipeline() = default;

   protected:
    explicit Pipeline(pipeline_id_t id);
    void setId(pipeline_id_t id);

    PipelineStructure pipelineStructure_;
};

class PipelineBuilder {
    friend class Renderer;

   public:
    PipelineBuilder& setShader(const AssetArg& shaderPath, ShaderStage type);
    virtual PipelineBuilder& setShader(const char* data, ShaderStage type) = 0;

    virtual Allocated<Pipeline> build() = 0;

    virtual ~PipelineBuilder() = default;

    PipelineBuilder(const PipelineBuilder&) = delete;
    PipelineBuilder(PipelineBuilder&&) = delete;
    PipelineBuilder& operator=(const PipelineBuilder&) = delete;
    PipelineBuilder& operator=(PipelineBuilder&&) = delete;

   protected:
    PipelineBuilder() = default;
};

class PipelineBuilderImpl : public PipelineBuilder {
    friend class RenderingManager;

   public:
    [[nodiscard]] pipeline_id_t id() const { return id_; };

    PipelineBuilderImpl& setId(pipeline_id_t id) {
        this->id_ = id;
        return *this;
    };

   protected:
    void addPipeline(Allocated<AllocatedPipeline> allocatedPipeline) {
        pipelines_->alloc(std::move(allocatedPipeline));
    };

   private:
    pipeline_id_t id_{PR_PIPELINE_NONE};

    pipeline_allocator_t* pipelines_;

    void setPipelineList(pipeline_allocator_t& pipelines) {
        pipelines_ = &pipelines;
    };
};

}  // namespace Presto
