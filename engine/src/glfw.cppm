export module presto.internal.glfw;

import presto.internal.window_data;

import presto.input;
import presto.runtime.window;

import presto.types;

export namespace Pr {

class GLFWAppWindow final : public Window {
   public:
    explicit GLFWAppWindow(const WindowProperties& props);
    ~GLFWAppWindow() override;

    void update() override;

    // Getters from WindowData
    [[nodiscard]] unsigned getWidth() const override;
    [[nodiscard]] unsigned getHeight() const override;
    [[nodiscard]] const VisualExtents& framebufferSize() const;

    void setVSync(bool vsync) override;
    bool vSyncEnabled() override;

    [[nodiscard]] void* getWindowHandle() const { return this->windowPtr_; }

    GLFWAppWindow(const GLFWAppWindow&) = delete;
    GLFWAppWindow(GLFWAppWindow&&) = delete;
    GLFWAppWindow& operator=(const GLFWAppWindow&) = delete;
    GLFWAppWindow& operator=(GLFWAppWindow&&) = delete;

   private:
    virtual void init(const WindowProperties& props);
    virtual void SetCallbacks();
    void shutdown() override;

    static bool s_GLFWInitialised;

    static Input::Key getPrestoKeyCode(int GLFWKeycode);

    Allocated<WindowData> windowData_{nullptr};
};

}  // namespace Pr
