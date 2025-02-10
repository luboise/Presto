#pragma once

#include "Presto/Assets/ImportTypes.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Rendering/VertexAttribute.h"
#include "Texture.h"

namespace Presto {

struct ShaderInfo {
    AttributeSet attributes;
    std::map<PR_STRING_ID, MaterialProperty> uniforms;
};

class PRESTO_API Shader {
    friend class Renderer;

   public:
    using uniform_name_t = PR_STRING_ID;

    virtual void setGlobalUniforms(GlobalUniforms uniforms) = 0;
    virtual void setObjectUniforms(ObjectUniforms uniforms) = 0;

    virtual void setUniform(uniform_name_t property,
                            Presto::uint32_t value) = 0;
    virtual void setUniform(uniform_name_t property, Presto::int32_t value) = 0;
    virtual void setUniform(uniform_name_t property,
                            Presto::float32_t value) = 0;

    virtual void setUniform(uniform_name_t property, Presto::vec2 value) = 0;
    virtual void setUniform(uniform_name_t property, Presto::vec3 value) = 0;
    virtual void setUniform(uniform_name_t property, Presto::vec4 value) = 0;

    virtual void setUniform(uniform_name_t property, Presto::mat4 value) = 0;

    [[nodiscard]] bool ready() const { return ready_; }

    virtual void use() const = 0;

    [[nodiscard]] ShaderInfo getInfo() const;

   protected:
    void setReady(bool ready) { ready_ = ready; }

    std::map<PR_STRING_ID, MaterialProperty> uniforms_;
    AttributeSet attributes_;

   private:
    bool ready_{false};
};
}  // namespace Presto
