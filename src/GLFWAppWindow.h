#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Presto/Window.h"

namespace Presto {
    class PRESTO_API GLFWAppWindow : public Window {
       public:
        GLFWAppWindow(const WindowProperties& props);
        virtual ~GLFWAppWindow();

        void RenderFrame() override;

        inline unsigned GetWidth() const override { return w_data.width; }
        inline unsigned GetHeight() const override { return w_data.height; }

        inline void SetCallbackFunction(const EventCallbackFn& fn) override {
            w_data.event_callback = fn;
        }

        void SetVSync(bool vsync) override;
        bool IsVSyncEnabled() override;

       private:
        double _glfwTime;

        virtual void Init(const WindowProperties& props);
        virtual void SetCallbacks();
        virtual void Shutdown() override;

        GLFWwindow* glfw_window;

        struct WindowData {
            std::string title;
            unsigned width;
            unsigned height;
            bool VSync;

            EventCallbackFn event_callback;
            Renderer* pRenderer;
        };

        WindowData w_data;
    };

}  // namespace Presto
