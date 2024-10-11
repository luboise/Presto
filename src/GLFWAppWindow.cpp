#include "GLFW/glfw3.h"
#include "Presto/Core.h"
#include "Presto/Events/ApplicationEvents.h"
#include "Presto/Events/KeyEvents.h"
#include "Presto/Events/MouseEvents.h"

#include "GLFWAppWindow.h"

#include "Presto/Rendering/Renderer.h"

#include "Presto/Rendering/RenderingManager.h"

#include "Editor/EditorOverlay.h"

namespace Presto {
    bool GLFWAppWindow::s_GLFWInitialised = false;

    Window* Window::Create(const WindowProperties& props) {
        return new GLFWAppWindow(props);
    }

    void GLFWAppWindow::Shutdown() {
        if (this->_windowPtr != nullptr) {
            glfwDestroyWindow(static_cast<GLFWwindow*>(this->_windowPtr));
            this->_windowPtr = nullptr;
        }
    }

    GLFWAppWindow::GLFWAppWindow(const WindowProperties& props) {
        this->GLFWAppWindow::Init(props);
    }

    GLFWAppWindow::~GLFWAppWindow() { this->GLFWAppWindow::Shutdown(); }

    void GLFWAppWindow::Init(const WindowProperties& props) {
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
            // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

            s_GLFWInitialised = true;
        }

        // The 2 nulls are fullscreen monitor choice and graphics library object
        // linking
        this->_windowPtr =
            (void*)glfwCreateWindow((int)props.width, (int)props.height,
                                    props.title.c_str(), nullptr, nullptr);

        // Set the window as the current context
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(this->_windowPtr));

        // Link props and glfw window
        glfwSetWindowUserPointer((GLFWwindow*)this->_windowPtr, &w_data);
        this->SetCallbacks();
        this->SetVSync(true);
    }

    void GLFWAppWindow::SetCallbacks() {
        // Set platform specific callbacks
        glfwSetWindowSizeCallback(
            (GLFWwindow*)this->_windowPtr,
            [](GLFWwindow* window, int new_width, int new_height) {
                WindowData& data =
                    *(WindowData*)(glfwGetWindowUserPointer(window));

                WindowResizeEvent e(new_width, new_height);
                data.width = new_width;
                data.height = new_height;
                data.event_callback(e);
            });

        glfwSetWindowCloseCallback(
            (GLFWwindow*)this->_windowPtr, [](GLFWwindow* window) {
                WindowData& data =
                    *(WindowData*)(glfwGetWindowUserPointer(window));
                WindowCloseEvent e = WindowCloseEvent();
                data.event_callback(e);
            });

        glfwSetKeyCallback(
            (GLFWwindow*)this->_windowPtr,
            [](GLFWwindow* window, int keycode, int scancode, int action,
               int mods) {
                WindowData& data =
                    *(WindowData*)(glfwGetWindowUserPointer(window));
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

        glfwSetMouseButtonCallback(
            (GLFWwindow*)this->_windowPtr,
            [](GLFWwindow* window, int button, int action, int mods) {
                WindowData& data =
                    *(WindowData*)(glfwGetWindowUserPointer(window));
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

        glfwSetCursorPosCallback(
            (GLFWwindow*)this->_windowPtr,
            [](GLFWwindow* window, double xpos, double ypos) {
                WindowData& data =
                    *(WindowData*)(glfwGetWindowUserPointer(window));
                MouseMovedEvent e((float)xpos, (float)ypos);
                data.event_callback(e);
            });

        glfwSetScrollCallback(
            (GLFWwindow*)this->_windowPtr,
            [](GLFWwindow* window, double xoffset, double yoffset) {
                WindowData& data =
                    *(WindowData*)(glfwGetWindowUserPointer(window));
                MouseScrolledEvent e((float)xoffset, (float)yoffset);
                data.event_callback(e);
            });

        glfwSetFramebufferSizeCallback(
            (GLFWwindow*)this->_windowPtr,
            [](GLFWwindow* window, int width, int height) {
                WindowData& data =
                    *(WindowData*)(glfwGetWindowUserPointer(window));

                data.pRenderer->framebufferResized();
            });
    }

    void GLFWAppWindow::RenderFrame() {
        glfwPollEvents();
        auto& rm = RenderingManager::Get();

        EditorOverlay::OnLoopStart();
        rm.Update();
        EditorOverlay::OnLoopEnd();

        rm.Flush();
    }

    void GLFWAppWindow::SetVSync(bool vsync) {
        if (vsync) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        this->w_data.VSync = vsync;
    }

    bool GLFWAppWindow::IsVSyncEnabled() { return w_data.VSync; }

}  // namespace Presto
