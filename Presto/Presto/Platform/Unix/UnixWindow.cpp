// #include "prpch.h"

#include "Presto/Core.h"
#include "Presto/Events/ApplicationEvents.h"
#include "Presto/Events/KeyEvents.h"
#include "Presto/Events/MouseEvents.h"

#include "UnixWindow.h"

#include "RenderingModule/Vulkan/VulkanRenderer.h"

namespace Presto {
    static bool s_GLFWInitialised = false;

    Window* Window::Create(const WindowProperties& props) {
        return new UnixWindow(props);
    }

    void UnixWindow::Shutdown() {
        if (this->glfw_window != nullptr) {
            this->_renderer->Shutdown();
            glfwDestroyWindow(this->glfw_window);
            this->glfw_window = nullptr;
        }
    }

    UnixWindow::UnixWindow(const WindowProperties& props) { this->Init(props); }

    UnixWindow::~UnixWindow() { this->Shutdown(); }

    void UnixWindow::Init(const WindowProperties& props) {
        this->w_data.title = props.title;
        this->w_data.width = props.width;
        this->w_data.height = props.height;

        PR_CORE_INFO("Creating window \"{}\" ({}x{})", props.title, props.width,
                     props.height);

        if (!s_GLFWInitialised) {
            int success = glfwInit();

            PR_CORE_ASSERT(
                success, "Unable to initialise GLFW. Program cannot continue.");

            // Disable OpenGL
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            s_GLFWInitialised = true;
        }

        // The 2 nulls are fullscreen monitor choice and graphics library object
        // linking
        this->glfw_window =
            glfwCreateWindow((int)props.width, (int)props.height,
                             props.title.c_str(), NULL, NULL);

        // Set the window as the current context
        glfwMakeContextCurrent(this->glfw_window);

        switch (props.render_library) {
            case VULKAN: {
                this->_renderer = new VulkanRenderer(this->glfw_window);
                PR_ASSERT(this->_renderer->IsInitialised());
                break;
            }
        }

        w_data.pRenderer = this->_renderer;

        // Link props and glfw window
        glfwSetWindowUserPointer(this->glfw_window, &w_data);
        this->SetCallbacks();
        this->SetVSync(true);
    }

    void UnixWindow::SetCallbacks() {
        // Set platform specific callbacks
        glfwSetWindowSizeCallback(this->glfw_window, [](GLFWwindow* window,
                                                        int new_width,
                                                        int new_height) {
            WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));

            WindowResizeEvent e(new_width, new_height);
            data.width = new_width;
            data.height = new_height;
            data.event_callback(e);
        });

        glfwSetWindowCloseCallback(this->glfw_window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
            WindowCloseEvent e = WindowCloseEvent();
            data.event_callback(e);
        });

        glfwSetKeyCallback(this->glfw_window, [](GLFWwindow* window,
                                                 int keycode, int scancode,
                                                 int action, int mods) {
            WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent e(keycode, 0);
                    data.event_callback(e);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent e(keycode);
                    data.event_callback(e);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent e(keycode, 1);
                    data.event_callback(e);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(this->glfw_window, [](GLFWwindow* window,
                                                         int button, int action,
                                                         int mods) {
            WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent e(button);
                    data.event_callback(e);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent e(button);
                    data.event_callback(e);
                    break;
                }
            }
        });

        glfwSetCursorPosCallback(this->glfw_window, [](GLFWwindow* window,
                                                       double xpos,
                                                       double ypos) {
            WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
            MouseMovedEvent e((float)xpos, (float)ypos);
            data.event_callback(e);
        });

        glfwSetScrollCallback(this->glfw_window, [](GLFWwindow* window,
                                                    double xoffset,
                                                    double yoffset) {
            WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
            MouseScrolledEvent e((float)xoffset, (float)yoffset);
            data.event_callback(e);
        });

        glfwSetFramebufferSizeCallback(
            this->glfw_window, [](GLFWwindow* window, int width, int height) {
                WindowData& data =
                    *(WindowData*)(glfwGetWindowUserPointer(window));

                data.pRenderer->framebufferResized();
            });
    }

    void UnixWindow::OnUpdate() {
        auto new_time = glfwGetTime();
        double delta = new_time - _glfwTime;

        // PRINT FPS
        // PR_CORE_TRACE("{:.2f} FPS", 1 / delta);

        _glfwTime = new_time;

        glfwPollEvents();
        _renderer->drawFrame();
    }

    void UnixWindow::SetVSync(bool vsync) {
        if (vsync) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        this->w_data.VSync = vsync;
    }

    bool UnixWindow::IsVSyncEnabled() { return w_data.VSync; }

}  // namespace Presto