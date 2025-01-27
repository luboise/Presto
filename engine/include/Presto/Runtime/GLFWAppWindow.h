#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Window.h"

namespace Presto {
    class Renderer;

    class GLFWAppWindow : public Window {
       public:
        explicit GLFWAppWindow(const WindowProperties& props);
        ~GLFWAppWindow() override;

        void update() override;

        [[nodiscard]] inline unsigned GetWidth() const override {
            return w_data.width;
        }
        [[nodiscard]] inline unsigned GetHeight() const override {
            return w_data.height;
        }

        inline void SetCallbackFunction(const EventCallbackFn& fn) override {
            w_data.event_callback = fn;
        }

        [[nodiscard]] inline GLFWwindow* getWindowHandle() const {
            return static_cast<GLFWwindow*>(this->_windowPtr);
        }
        explicit operator GLFWwindow*() const {
            return this->getWindowHandle();
        }
        explicit operator const GLFWwindow*() const {
            return this->getWindowHandle();
        }

        void SetVSync(bool vsync) override;
        bool IsVSyncEnabled() override;

       private:
        double _glfwTime;

        virtual void init(const WindowProperties& props);
        virtual void SetCallbacks();
        void shutdown() override;

        static bool s_GLFWInitialised;

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
