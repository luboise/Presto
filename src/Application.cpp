
#include "Presto/Application.h"

#include "GLFW/glfw3.h"
#include "Presto/Rendering/RenderingManager.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Presto {
    Application::Application() {
        app_window = std::unique_ptr<Window>(Window::Create());
        app_window->SetCallbackFunction(BIND_EVENT_FN(Application::OnEvent));

        // Set up modules
        auto ptr = static_cast<GLFWwindow*>(this->app_window->getWindowPtr());
        RenderingManager::F_INIT(Renderer::VULKAN, ptr);
        RenderingManager::SetCamera(_mainCamera);
    }

    Application::~Application() { /*this->app_window->Shutdown();*/
        this->app_window->Shutdown();
        this->app_window.release();
    };

    void Application::Run() {
        while (app_running) {
            // Calculate delta

            double newTime = glfwGetTime();
            _delta = newTime - _currentTime;
            _currentTime = newTime;

            // PRINT FPS
            // PR_CORE_TRACE("{:.2f} FPS", 1 / delta);

            // TODO: Create new entities

            // Run user logic
            GameLoop();
            RunSystems();

            app_window->RenderFrame();
        }
    }

    Camera& Application::GetMainCamera() { return _mainCamera; }

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
