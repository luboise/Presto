#pragma once
// #include "prpch.h"

#include "Presto/Modules/Module.h"

namespace Presto {
    class PRESTO_API GraphicsModule : Module {
       public:
        virtual Module::MODULE_LAUNCH_CODE start();
        virtual void stop();

        enum GRAPHICS_RENDERER { VULKAN, OPENGL, DIRECTX };
        void setRenderer(GRAPHICS_RENDERER new_renderer);

       private:
        GRAPHICS_RENDERER _renderer;
    };

}  // namespace Presto