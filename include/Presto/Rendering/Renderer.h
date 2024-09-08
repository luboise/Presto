#pragma once

#include "Presto/Objects/Entity.h"
#include "Presto/Rendering/Camera.h"

namespace Presto {
    class PRESTO_API Renderer {
       public:
        enum RENDER_LIBRARY { VULKAN, OPENGL, DIRECTX };

        virtual void AddToRenderPool(entity_t entity_ptr) = 0;
        virtual void draw(entity_t entity_ptr) = 0;
        virtual void nextFrame() = 0;

        void setCamera(Camera& newCam) { _renderCamera = &newCam; }

        void framebufferResized() { this->_framebufferResized = true; }

        virtual ~Renderer() {};

       protected:
        bool _framebufferResized = false;
        Camera* _renderCamera;
    };
}  // namespace Presto
