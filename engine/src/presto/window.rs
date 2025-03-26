extern crate glfw;

use crate::presto::events;

use events::Event;

enum RenderLibrary {
    OpenGL,
    Vulkan,
}

struct WindowManager {
    instance: glfw::Glfw,
}

type VisualExtentSize = u32;
const DEFAULT_WIDTH: VisualExtentSize = 2560;
const DEFAULT_HEIGHT: VisualExtentSize = 1440;

struct VisualExtents {
    width: VisualExtentSize,
    height: VisualExtentSize,
}
impl Default for VisualExtents {
    fn default() -> Self {
        VisualExtents {
            width: DEFAULT_WIDTH,
            height: DEFAULT_HEIGHT,
        }
    }
}

struct WindowProperties {
    title: String,
    extents: VisualExtents,
    render_library: RenderLibrary,
}
impl Default for WindowProperties {
    fn default() -> Self {
        WindowProperties {
            title: String::from("Untitled Presto Application"),
            extents: Default::default(),
            render_library: RenderLibrary::OpenGL,
        }
    }
}

// #include "Presto/Core/Constants.h"
// #include "Presto/Types/CoreTypes.h"

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

type EventCallbackFn = fn(&dyn Event);

pub trait Window {
    fn shutdown();
    fn update();
    fn get_width() -> VisualExtentSize;
    fn get_height() -> VisualExtentSize;
    fn set_callback(callback: &EventCallbackFn);
    fn set_vsync(vsync: bool);
    fn v_sync_enabled();
}
