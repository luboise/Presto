export module presto.runtime.window;

import std;

import presto.types;
import presto.core.constants;

constexpr auto DEFAULT_WIDTH = 2560;
constexpr auto DEFAULT_HEIGHT = 1440;

export namespace Pr {

struct WindowProperties {
    Pr::string title{"Untitled Presto application"};
    VisualExtents extents{.width = DEFAULT_WIDTH, .height = DEFAULT_HEIGHT};
    RENDER_LIBRARY render_library{UNSET};
};

// Abstracted window interface to be implemented per platform
class Window {
   public:
    using WindowPtr = std::unique_ptr<Window>;

    Window() = default;
    virtual ~Window() = default;

    virtual void shutdown() = 0;

    // TODO: Move this into the engine
    virtual void update() = 0;

    [[nodiscard]] virtual unsigned getWidth() const = 0;
    [[nodiscard]] virtual unsigned getHeight() const = 0;

    virtual void setVSync(bool vsync) = 0;
    virtual bool vSyncEnabled() = 0;

    virtual void* getWindowPtr() { return windowPtr_; };

    Window(const Window&) = default;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = default;
    Window& operator=(Window&&) = delete;

   protected:
    void* windowPtr_{};
};

using WindowPtr = Window::WindowPtr;

}  // namespace Pr
