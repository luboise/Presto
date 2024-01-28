#pragma once
// #include "prpch.h"

#include "Presto/Modules/Module.h"

namespace Presto {
    struct ShaderMatrices {
        glm::mat4 modelView;
        glm::mat4 projection;
    };

    class PRESTO_API Renderer : public Module {
       public:
        virtual PR_RESULT drawFrame() = 0;

        void framebufferResized() { this->_framebufferResized = true; }

       protected:
        bool _framebufferResized = false;
    };
}  // namespace Presto