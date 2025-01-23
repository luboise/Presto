#pragma once

#include "Presto/Rendering/RenderTypes.h"
#include "Window.h"

namespace Presto {
    class Renderer;

    class GLFWAppWindow : public Window {
       public:
        explicit GLFWAppWindow(const WindowProperties& props);
        ~GLFWAppWindow() override;

        void update() override;

        [[nodiscard]] inline unsigned GetWidth() const override {
            return windowData_.window_size.width;
        }
        [[nodiscard]] inline unsigned GetHeight() const override {
            return windowData_.window_size.height;
        }

        inline void SetCallbackFunction(const EventCallbackFn& fn) override {
            windowData_.event_callback = fn;
        }

        [[nodiscard]] inline void* getWindowHandle() const {
            return this->_windowPtr;
        }

        /*
explicit operator GLFWwindow*() const {
    return this->getWindowHandle();
}
        */

        /*
                explicit operator const GLFWwindow*() const {
                    return this->getWindowHandle();
                }
                        */

        [[nodiscard]] const VisualExtents& getFramebufferSize() const {
            return windowData_.framebuffer_size;
        }

        void SetVSync(bool vsync) override;
        bool IsVSyncEnabled() override;

       private:
        double _glfwTime;

        virtual void init(const WindowProperties& props);
        virtual void SetCallbacks();
        void shutdown() override;

        static bool s_GLFWInitialised;

        static Input::Key getPrestoKeyCode(int GLFWKeycode);

        struct WindowData {
            std::string title;

            VisualExtents window_size;
            VisualExtents framebuffer_size;

            bool VSync{false};

            EventCallbackFn event_callback;
            Renderer* pRenderer{nullptr};
        };

        WindowData windowData_;
    };

}  // namespace Presto
