module presto.runtime.window.internal;

import std;
import presto.internal.glfw;

Pr::Window::WindowPtr Pr::Create(const WindowProperties& props) {
    return std::make_unique<GLFWAppWindow>(props);
}
