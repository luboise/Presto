#include "Presto/Runtime/GLFWAppWindow.h"
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Presto/Core/KeyCodes.h"
#include "Presto/Runtime.h"
#include "Presto/Runtime/Events/ApplicationEvents.h"
#include "Presto/Runtime/Events/KeyEvents.h"
#include "Presto/Runtime/Events/MouseEvents.h"

#include "Presto/Rendering/Renderer.h"

namespace Presto {
    bool GLFWAppWindow::s_GLFWInitialised = false;

    Window::WindowPtr Window::create(const WindowProperties& props) {
        return std::make_unique<GLFWAppWindow>(props);
    }

    void GLFWAppWindow::shutdown() {
        if (this->windowPtr_ != nullptr) {
            glfwDestroyWindow(static_cast<GLFWwindow*>(this->windowPtr_));
            this->windowPtr_ = nullptr;
        }
    }

    GLFWAppWindow::GLFWAppWindow(const WindowProperties& props) { init(props); }

    GLFWAppWindow::~GLFWAppWindow() { this->GLFWAppWindow::shutdown(); }

    void GLFWAppWindow::init(const WindowProperties& props) {
        this->windowData_.title = props.title;

        this->windowData_.window_size = props.extents;
        this->windowData_.framebuffer_size = props.extents;

        PR_CORE_INFO("Creating window \"{}\" ({}x{})", props.title,
                     props.extents.width, props.extents.height);

        if (!s_GLFWInitialised) {
            int success = glfwInit();

            PR_CORE_ASSERT(
                success, "Unable to initialise GLFW. Program cannot continue.");

            // Disable OpenGL
            // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

            s_GLFWInitialised = true;
        }

        // The 2 nulls are fullscreen monitor choice and graphics library object
        // linking
        GLFWwindow* new_window{glfwCreateWindow(
            (int)props.extents.width, (int)props.extents.height,
            props.title.c_str(), nullptr, nullptr)};

        this->windowPtr_ = static_cast<void*>(new_window);

        // Set aspect ratio to 16:9
        glfwSetWindowAspectRatio(static_cast<GLFWwindow*>(this->windowPtr_),
                                 (int)props.extents.width,
                                 (int)props.extents.height);

        glfwSetWindowSizeLimits(static_cast<GLFWwindow*>(this->windowPtr_), 320,
                                180, GLFW_DONT_CARE, GLFW_DONT_CARE);

        // Set the window as the current context
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(this->windowPtr_));

        // Link props and glfw window
        glfwSetWindowUserPointer(new_window, &windowData_);
        this->SetCallbacks();
        this->setVSync(true);
    }

    void GLFWAppWindow::SetCallbacks() {
        GLFWwindow* window_ptr{static_cast<GLFWwindow*>(this->windowPtr_)};

        glfwSetWindowCloseCallback(window_ptr, [](GLFWwindow* window) {
            WindowData& data{
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window))};
            WindowCloseEvent e = WindowCloseEvent();
            data.event_callback(e);
        });

        glfwSetKeyCallback(
            static_cast<GLFWwindow*>(this->windowPtr_),
            [](GLFWwindow* window, int keycode, int scancode, int action,
               int /*mods*/) {
                WindowData& data{*static_cast<WindowData*>(
                    glfwGetWindowUserPointer(window))};

                Input::Key presto_key_code{getPrestoKeyCode(keycode)};

                // Skip unsupported keys
                if (presto_key_code == Input::Key::INVALID_KEY) {
                    return;
                }

                switch (action) {
                    case GLFW_PRESS: {
                        KeyEvent e(presto_key_code,
                                   KeyEvent::KeyEventType::PRESSED);
                        data.event_callback(e);
                        break;
                    }
                    case GLFW_RELEASE: {
                        KeyEvent e(presto_key_code,
                                   KeyEvent::KeyEventType::RELEASED);
                        data.event_callback(e);
                        break;
                    }
                    default:
                        return;
                }
            });

        glfwSetMouseButtonCallback(
            window_ptr,
            [](GLFWwindow* window, int button, int action, int /*mods*/) {
                WindowData& data{*static_cast<WindowData*>(
                    glfwGetWindowUserPointer(window))};

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
                    default:
                        return;
                }
            });

        glfwSetCursorPosCallback(
            window_ptr, [](GLFWwindow* window, double xpos, double ypos) {
                WindowData& data{*static_cast<WindowData*>(
                    glfwGetWindowUserPointer(window))};
                MouseMovedEvent e((float)xpos, (float)ypos);
                data.event_callback(e);
            });

        glfwSetScrollCallback(
            window_ptr, [](GLFWwindow* window, double xoffset, double yoffset) {
                WindowData& data{*static_cast<WindowData*>(
                    glfwGetWindowUserPointer(window))};
                MouseScrolledEvent e((float)xoffset, (float)yoffset);
                data.event_callback(e);
            });

        // Set platform specific callbacks
        glfwSetWindowSizeCallback(
            window_ptr, [](GLFWwindow* window, int new_width, int new_height) {
                WindowData& data{*static_cast<WindowData*>(
                    glfwGetWindowUserPointer(window))};

                WindowResizeEvent e(new_width, new_height);

                data.window_size = {
                    .width = static_cast<std::uint16_t>(new_width),
                    .height = static_cast<std::uint16_t>(new_height),
                };
                data.event_callback(e);
            });

        glfwSetFramebufferSizeCallback(
            window_ptr, [](GLFWwindow* window, int new_width, int new_height) {
                WindowData& data{*static_cast<WindowData*>(
                    glfwGetWindowUserPointer(window))};

                data.framebuffer_size = {
                    .width = static_cast<std::uint16_t>(new_width),
                    .height = static_cast<std::uint16_t>(new_height),
                };
                FramebufferResizedEvent e(new_width, new_height);
                data.event_callback(e);
            });
    }

    void GLFWAppWindow::update() { glfwPollEvents(); }

    void GLFWAppWindow::setVSync(bool vsync) {
        if (vsync) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        this->windowData_.VSync = vsync;
    }

    bool GLFWAppWindow::vSyncEnabled() { return windowData_.VSync; }

    Input::Key GLFWAppWindow::getPrestoKeyCode(int GLFWKeycode) {
        switch (GLFWKeycode) {
            case GLFW_KEY_UP:
                return Input::Key::UP_ARROW;
            case GLFW_KEY_DOWN:
                return Input::Key::DOWN_ARROW;
            case GLFW_KEY_LEFT:
                return Input::Key::LEFT_ARROW;
            case GLFW_KEY_RIGHT:
                return Input::Key::RIGHT_ARROW;
            case GLFW_KEY_SPACE:
                return Input::Key::SPACEBAR;
            default:
                return Input::Key::INVALID_KEY;
        }
    };

}  // namespace Presto
