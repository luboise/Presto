#pragma once

#include "Presto/Core.h"
#include "Presto/Events/Event.h"

#include "Modules/RenderingModule/_Renderer.h"

namespace Presto {
    enum RENDER_LIBRARY { VULKAN, OPENGL, DIRECTX };

    struct WindowProperties {
        std::string title;
        unsigned height;
        unsigned width;
        RENDER_LIBRARY render_library;

        // Default constructor
        WindowProperties(
            const std::string& d_title = "Untitled Presto application",
            unsigned d_width = 1280, unsigned d_height = 720)
            : title(d_title),
              width(d_width),
              height(d_height),
              render_library(VULKAN) {}
    };

    // Abstracted window interface to be implemented per platform
    class PRESTO_API Window {
       public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window(){};
        virtual void Shutdown() = 0;

        virtual void RenderFrame() = 0;

        virtual unsigned GetWidth() const = 0;
        virtual unsigned GetHeight() const = 0;

        virtual void SetCallbackFunction(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool vsync) = 0;
        virtual bool IsVSyncEnabled() = 0;

        virtual GLFWwindow* GetWindowPtr();

        // Window create function that must be implemented per platform
        // Uses default props if unspecified
        static Window* Create(
            const WindowProperties& props = WindowProperties());

        Renderer* _renderer;
    };

}  // namespace Presto