#pragma once

#include "Presto/Rendering/RenderTypes.h"
#include "Texture.h"

namespace Presto {
class PRESTO_API Shader {
    friend class Renderer;

   public:
    using uniform_name_t = PR_STRING_ID;

    virtual void setGlobalUniforms(GlobalUniforms uniforms) = 0;
    virtual void setObjectUniforms(ObjectUniforms uniforms) = 0;

    virtual void setUniform(uniform_name_t property, std::uint8_t value) = 0;
    virtual void setUniform(uniform_name_t property, int value) = 0;
    virtual void setUniform(uniform_name_t property, float value) = 0;
    virtual void setUniform(uniform_name_t property, Presto::mat4 value) = 0;

    virtual void setTexture(uniform_name_t property, Texture value) = 0;

    [[nodiscard]] bool ready() const { return ready_; }

    virtual void use() const = 0;

   protected:
    void setReady(bool ready) { ready_ = ready; }

   private:
    bool ready_{false};
};
}  // namespace Presto
