#pragma once

#include "Presto/Modules/Module.h"

namespace Presto {
    class PRESTO_API Renderer : public Module {
       public:
        virtual void Update() = 0;

        virtual void AddToRenderPool(entity_t entity_ptr) = 0;
        virtual void DrawEntity(entity_t entity_ptr) = 0;

        void framebufferResized() { this->_framebufferResized = true; }

       protected:
        bool _framebufferResized = false;
        std::vector<RenderLayer> _layers;
    };
}  // namespace Presto