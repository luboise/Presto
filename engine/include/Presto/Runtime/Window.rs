extern crate glfw;

#include "Presto/Core/Constants.h"
#include "Presto/Types/CoreTypes.h"

const DEFAULT_WIDTH: i32 = 2560;
const DEFAULT_HEIGHT: i32 = 1440;

class Event;

struct WindowProperties {
    std::string title{"Untitled Presto application"};
    VisualExtents extents{.width = DEFAULT_WIDTH, .height = DEFAULT_HEIGHT};
    RENDER_LIBRARY render_library{UNSET};
};


use glfw::{Action, Context, Key};

fn main() {
    let mut glfw = glfw::init(glfw::fail_on_errors).unwrap();

    let (mut window, events) = glfw
        .create_window(300, 300, "Hello this is window", glfw::WindowMode::Windowed)
        .expect("Failed to create GLFW window.");

    window.set_key_polling(true);
    window.make_current();

    while !window.should_close() {
        glfw.poll_events();
        for (_, event) in glfw::flush_messages(&events) {
            handle_window_event(&mut window, event);
        }
    }
}

fn handle_window_event(window: &mut glfw::Window, event: glfw::WindowEvent) {
    match event {
        glfw::WindowEvent::Key(Key::Escape, _, Action::Press, _) => window.set_should_close(true),
        _ => {}
    }
}

// Abstracted window interface to be implemented per platform
class Window {
    friend class Application;

   public:
    using WindowPtr = std::unique_ptr<Window>;
    using EventCallbackFn = std::function<void(Event&)>;

    Window() = default;
    virtual ~Window() = default;

    virtual void shutdown() = 0;

    // TODO: Move this into the engine
    virtual void update() = 0;

    [[nodiscard]] virtual unsigned getWidth() const = 0;
    [[nodiscard]] virtual unsigned getHeight() const = 0;

    virtual void setCallbackFunction(const EventCallbackFn& callback) = 0;
    virtual void setVSync(bool vsync) = 0;
    virtual bool vSyncEnabled() = 0;

    virtual void* getWindowPtr() { return windowPtr_; };

    // Window create function that must be implemented per platform
    // Uses default props if unspecified
    static WindowPtr create(const WindowProperties& props = WindowProperties());

    Window(const Window&) = default;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = default;
    Window& operator=(Window&&) = delete;

   protected:
    void* windowPtr_{};
};

using WindowPtr = Window::WindowPtr;

}  // namespace Presto
