module presto.internal.create_window;

import std;
import presto.internal.glfw;

Pr::Window::WindowPtr Pr::CreateWindow(const WindowProperties& props) {
    return std::make_unique<GLFWAppWindow>(props);
}
