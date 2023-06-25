#pragma once

#include <GLFW/glfw3.h>

#include "Presto/Window.h"
#include "prpch.h"

namespace Presto {
    class WindowsWindow : public Window {
       public:
        WindowsWindow(const WindowProperties& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned GetWidth() const override { return w_data.width; }
        inline unsigned GetHeight() const override { return w_data.height; }

        inline void SetCallbackFunction(const EventCallbackFn& fn) override {
            w_data.event_callback = fn;
        }

        void SetVSync(bool vsync) override;
        bool IsVSyncEnabled() override;

       private:
        virtual void Init(const WindowProperties& props);
        virtual void Shutdown();

        GLFWwindow* glfw_window;

        struct WindowData {
            std::string title;
            unsigned width;
            unsigned height;
            bool VSync;

            EventCallbackFn event_callback;
        };

        WindowData w_data;
    };

}  // namespace Presto