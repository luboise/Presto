#pragma once

#include <map>
#include "Presto/Components/Renderable.h"

#include "Presto/Rendering/Camera.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {

    typedef Renderable* draw_info_key;
    typedef std::map<draw_info_key, DrawInfo> DrawInfoMap;

    class PRESTO_API Renderer {
       public:
        enum RENDER_LIBRARY { VULKAN, OPENGL, DIRECTX };

        virtual void AddToRenderPool(draw_info_key) = 0;
        virtual void draw(draw_info_key) = 0;
        virtual void nextFrame() = 0;

        void setCamera(Camera& newCam) { _renderCamera = &newCam; }

        void framebufferResized() { this->_framebufferResized = true; }

        virtual ~Renderer() {};

       protected:
        DrawInfoMap _drawInfoMap;

        bool _framebufferResized = false;
        Camera* _renderCamera;
    };
}  // namespace Presto
