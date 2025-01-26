#pragma once

#include "Texture.h"

namespace Presto {
    class PRESTO_API Shader {
        friend class Renderer;

        using uniform_name_t = PR_STRING_ID;

       public:
        virtual void setUniform(uniform_name_t property, int value) = 0;
        virtual void setUniform(uniform_name_t property, float value) = 0;
        virtual void setUniform(uniform_name_t property,
                                Presto::mat4 value) = 0;

        virtual void setTexture(uniform_name_t property, Texture value) = 0;

       private:
    };
}  // namespace Presto
