#pragma once

#include "Presto/Modules/Module.h"

#include "Presto/Modules/ObjectsModule/_EntityHeader.h"

#include "Types/Camera.h"

namespace Presto {
    class PRESTO_API Renderer : public Module {
       public:
        virtual void Update() = 0;

        virtual void AddToRenderPool(entity_t entity_ptr) = 0;

        void framebufferResized() { this->_framebufferResized = true; }

       protected:
        bool _framebufferResized = false;
        Camera _renderCamera;
    };
}  // namespace Presto