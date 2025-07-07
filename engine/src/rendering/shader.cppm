module;
#include "presto/platform.h"

export module presto.internal.rendering:shader;

import :renderer;
import :vertex;
import :types;

export namespace Pr {

struct ShaderInfo {
    AttributeSet attributes;
    std::map<Pr::string, MaterialProperty> uniforms;
};

class PRESTO_API Shader {
    friend class Renderer;

   public:
    virtual void setGlobalUniforms(GlobalUniforms uniforms) = 0;
    virtual void setObjectUniforms(ObjectUniforms uniforms) = 0;

    virtual void setUniform(uniform_name_t property, Pr::uint32_t value) = 0;
    virtual void setUniform(uniform_name_t property, Pr::int32_t value) = 0;
    virtual void setUniform(uniform_name_t property, Pr::float32_t value) = 0;

    virtual void setUniform(uniform_name_t property, Pr::vec2 value) = 0;
    virtual void setUniform(uniform_name_t property, Pr::vec3 value) = 0;
    virtual void setUniform(uniform_name_t property, Pr::vec4 value) = 0;

    virtual void setUniform(uniform_name_t property, Pr::mat4 value) = 0;

    [[nodiscard]] bool ready() const { return ready_; }

    virtual void use() const = 0;

    [[nodiscard]] ShaderInfo getInfo() const;

   protected:
    void setReady(bool ready) { ready_ = ready; }

    std::map<Pr::string, MaterialProperty> uniforms_;
    AttributeSet attributes_;

   private:
    bool ready_{false};
};

}  // namespace Pr
