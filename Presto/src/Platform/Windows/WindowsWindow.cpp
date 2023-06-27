#include "prpch.h"

#include "Presto/Core.h"
#include "WindowsWindow.h"

namespace Presto {
    static bool s_GLFWInitialised = false;

    Window* Window::Create(const WindowProperties& props) {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProperties& props) {
        this->Init(props);
    }

    WindowsWindow::~WindowsWindow() { this->Shutdown(); }

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
        this->w_data.title = props.title;
        this->w_data.width = props.width;
        this->w_data.height = props.height;

        PR_CORE_INFO("Creating window \"{}\" ({}x{})", props.title, props.width,
                     props.height);

        if (!s_GLFWInitialised) {
            int success = glfwInit();

            PR_CORE_ASSERT(
                success, "Unable to initialise GLFW. Program cannot continue.");

            s_GLFWInitialised = true;
        }

        // The 2 nulls are fullscreen monitor choice and graphics library object
        // linking
        this->glfw_window =
            glfwCreateWindow((int)props.width, (int)props.height,
                             props.title.c_str(), NULL, NULL);

        // Set the window as the current context
        glfwMakeContextCurrent(this->glfw_window);

        // Link props and glfw window
        glfwSetWindowUserPointer(this->glfw_window, &w_data);
        this->SetVSync(true);
    }

    void WindowsWindow::Shutdown() { glfwDestroyWindow(this->glfw_window); }

}  // namespace Presto