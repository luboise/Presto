#include "WindowsWindow.h"

#include "Presto/Core.h"
#include "prpch.h"

namespace Presto {
    static bool s_GLFWInitialised = false;

    Window* Window::Create(const WindowProperties& props) {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProperties& props) { Init(props); }

    void WindowsWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(glfw_window);
    }

    void WindowsWindow::SetVSync(bool vsync) {
        if (vsync) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        this->w_data.VSync = vsync;
    }

    bool WindowsWindow::IsVSyncEnabled() { return w_data.VSync; }

    void WindowsWindow::Init(const WindowProperties& props) {
        w_data.title = props.title;
        w_data.width = props.width;
        w_data.height = props.height;

        PR_CORE_INFO("Creating window \"{} ({}x{})", props.title, props.width,
                     props.height);

        if (!s_GLFWInitialised) {
            int success = glfwInit();

            PR_CORE_ASSERT(
                success, "Unable to initialise GLFW. Program cannot continue.");

            s_GLFWInitialised = true;
        }

        // The 2 nulls are fullscreen monitor choice and graphics library object
        // linking
        glfw_window = glfwCreateWindow((int)props.width, (int)props.height,
                                       props.title.c_str(), NULL, NULL);

        // Set the window as the current context
        glfwMakeContextCurrent(glfw_window);

        // Link props and glfw window
        glfwSetWindowUserPointer(glfw_window, &w_data);
        SetVSync(true);
    }

    void WindowsWindow::Shutdown() { glfwDestroyWindow; }

}  // namespace Presto