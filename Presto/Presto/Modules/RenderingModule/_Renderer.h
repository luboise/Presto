#pragma once

#include "Presto/Modules/Module.h"

#include "Presto/Modules/ObjectsModule/_EntityHeader.h"

#include "Presto/Common/Camera.h"

namespace Presto {
    class PRESTO_API Renderer {
       public:
        enum RENDER_LIBRARY { VULKAN, OPENGL, DIRECTX };

        virtual void AddToRenderPool(entity_t entity_ptr) = 0;
        virtual void draw(entity_t entity_ptr) = 0;
        virtual void nextFrame() = 0;

        void setCamera(Camera& newCam) { _renderCamera = &newCam; }

        void framebufferResized() { this->_framebufferResized = true; }

       protected:
        bool _framebufferResized = false;
        Camera* _renderCamera;
    };
}  // namespace Presto