#include "Presto/Rendering/PipelineBuilder.h"

#include "Rendering/OpenGL/OpenGLRenderer.h"
#include "Rendering/OpenGL/OpenGLShader.h"
#include "Rendering/OpenGL/utils.h"

namespace Presto {
class OpenGLPipelineBuilder final : public PipelineBuilder {
    friend class OpenGLRenderer;

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

                vertexShader_ = vs;
                break;
            }
            case ShaderStage::FRAGMENT: {
                GLuint fs{glCreateShader(GL_FRAGMENT_SHADER)};

                glShaderSource(fs, 1, &data, nullptr);
                glCompileShader(fs);

                PR_CORE_ASSERT(OpenGLUtils::ShaderCompiledCorrectly(fs),
                               "Fragment shader failed to compile.");

                fragmentShader_ = fs;
                break;
            }
            default: {
                PR_CORE_ERROR("Invalid shader set in OpenGL Pipeline.");
            }
        }

        return *this;
    };

    PipelineStructure build() override {
        OpenGLShader shader{vertexShader_, fragmentShader_};

        glDeleteShader(vertexShader_);
        vertexShader_ = INVALID_SHADER_ID;

        glDeleteShader(fragmentShader_);
        fragmentShader_ = INVALID_SHADER_ID;

        OpenGLPipeline pipeline{shader};
    };

   private:
    OpenGLRenderer* renderer_{nullptr};

    GLuint vertexShader_{INVALID_SHADER_ID};
    GLuint fragmentShader_{INVALID_SHADER_ID};

    explicit OpenGLPipelineBuilder(OpenGLRenderer* renderer)
        : renderer_(renderer), shaderProgram_(glCreateProgram()) {};

    ~OpenGLPipelineBuilder() {}
};
}  // namespace Presto
