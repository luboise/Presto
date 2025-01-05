#include "Presto/Runtime/Application.h"
#include "Presto/Rendering/Camera.h"
#include "Presto/Runtime/Events/ApplicationEvents.h"

#include "GLFW/glfw3.h"

#include "Presto/Modules/RenderingManager.h"
#include "Presto/Runtime/GLFWAppWindow.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Presto {
    Application::Application() {
        // TODO: Fix this to be injected

        auto* app_window = (dynamic_cast<GLFWAppWindow*>(Window::Create()));

        this->_app_window = std::unique_ptr<GLFWAppWindow>(app_window);
        // this->_app_window = new GLFWAppWindow();
        _app_window->SetCallbackFunction(BIND_EVENT_FN(Application::OnEvent));

        RenderingManager::setRenderLibrary(OPENGL);
        RenderingManager::setWindow(app_window);
        RenderingManager::setWindow(app_window);

        RenderingManager::Init();

        // TODO: Make this use an actual component
        _mainCamera = new Camera();
        RenderingManager::Get().setCamera(*_mainCamera);
    }

    Application::~Application() { /*this->app_window->Shutdown();*/
        this->_app_window->Shutdown();
        // this->_app_window.release();
    };

    void Application::Run() {
        while (app_running) {
            // Calculate delta

            double newTime = glfwGetTime();
            _delta = newTime - _currentTime;
            _currentTime = newTime;

            // PRINT FPS
            // PR_CORE_TRACE("{:.2f} FPS", 1 / _delta);

            // TODO: Create new entities

            // Run user logic
            GameLoop();
            RunSystems();

            _app_window->Update();
            RenderingManager::Get().Update();
        }
    }

    Camera& Application::GetMainCamera() { return *_mainCamera; }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(
            BIND_EVENT_FN(Application::OnWindowClose));

        PR_CORE_TRACE("{}", e.ToString());
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        this->app_running = false;
        return true;
    }

    void Application::RunSystems() {
        for (auto& system : _systems) {
            system->Update();
        }
    }
}  // namespace Presto
