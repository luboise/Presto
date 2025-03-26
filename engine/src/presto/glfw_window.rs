mod window;

// #include "Presto/Core/KeyCodes.h"
// #include "Window.h"


struct GLFWWindow{}

impl Window for GLFWWindow{

}

class GLFWAppWindow : public Window {

    [[nodiscard]] unsigned getWidth() const override {
        return windowData_.window_size.width;
    }
    [[nodiscard]] unsigned getHeight() const override {
        return windowData_.window_size.height;
    }

    void setCallbackFunction(const EventCallbackFn& fn) override {
        windowData_.event_callback = fn;
    }

    [[nodiscard]] void* getWindowHandle() const { return this->windowPtr_; }

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

    void setVSync(bool vsync) override;
    bool vSyncEnabled() override;

    GLFWAppWindow(const GLFWAppWindow&) = default;
    GLFWAppWindow(GLFWAppWindow&&) = delete;
    GLFWAppWindow& operator=(const GLFWAppWindow&) = default;
    GLFWAppWindow& operator=(GLFWAppWindow&&) = delete;

   private:
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
