#include "Presto/Rendering/PipelineBuilder.h"
#include "Rendering/PipelineBuilderImpl.h"

#include "Rendering/OpenGL/OpenGLPipeline.h"
#include "Rendering/OpenGL/utils.h"

namespace Presto {

class OpenGLPipelineBuilder final : public PipelineBuilderImpl {
    friend class OpenGLRenderer;
    friend class RenderingManager;

    static constexpr auto INVALID_SHADER_ID = 0;

   public:
    PipelineBuilder& setShader(const char* data, ShaderStage type) override {
        switch (type) {
            case ShaderStage::VERTEX: {
                GLuint vs{glCreateShader(GL_VERTEX_SHADER)};

                glShaderSource(vs, 1, &data, nullptr);
                glCompileShader(vs);

                PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(vs),
                               "Vertex shader failed to compile.");

                vertexShader_ = ShaderAllocation{vs};
                break;
            }
            case ShaderStage::FRAGMENT: {
                GLuint fs{glCreateShader(GL_FRAGMENT_SHADER)};

                glShaderSource(fs, 1, &data, nullptr);
                glCompileShader(fs);

                PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(fs),
                               "Fragment shader failed to compile.");

                fragmentShader_ = ShaderAllocation{fs};
                break;
            }
            default: {
                PR_CORE_ERROR("Invalid shader set in OpenGL Pipeline.");
            }
        }

        return *dynamic_cast<PipelineBuilder*>(this);
    };

    Allocated<Pipeline> build() override {
        PR_ASSERT(vertexShader_.id != 0,
                  "Vertex shader was unassigned when building the pipeline.");

        PR_ASSERT(fragmentShader_.id != 0,
                  "Fragment shader was unassigned when building the pipeline.");

        Allocated<Pipeline> pipeline{
            new OpenGLPipeline(vertexShader_.id, fragmentShader_.id)};

        return pipeline;
    };

   private:
    OpenGLPipelineBuilder() = default;

    // OpenGLRenderer* renderer_{nullptr};

    struct ShaderAllocation {
        GLuint id{0};

        explicit ShaderAllocation(GLuint id);
        ~ShaderAllocation() { glDeleteShader(id); }

        ShaderAllocation(const ShaderAllocation&) = delete;
        ShaderAllocation& operator=(const ShaderAllocation&) = delete;
        ShaderAllocation(ShaderAllocation&&) = default;
        ShaderAllocation& operator=(ShaderAllocation&&) = default;
    };

    ShaderAllocation vertexShader_{INVALID_SHADER_ID};
    ShaderAllocation fragmentShader_{INVALID_SHADER_ID};
};

}  // namespace Presto
