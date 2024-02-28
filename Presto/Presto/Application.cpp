
#include "Application.h"
#include "Modules/_ModulesHeader.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Presto {
    Application::Application() {
        app_window = std::unique_ptr<Window>(Window::Create());
        app_window->SetCallbackFunction(BIND_EVENT_FN(Application::OnEvent));

        // Set up modules
        auto ptr = static_cast<GLFWwindow*>(this->app_window->getWindowPtr());
        _modules.push_back(new RenderingManager(ptr, app_window->_renderer));
    }

    Application::~Application() { /*this->app_window->Shutdown();*/
        this->app_window->Shutdown();
        this->app_window.release();
    };

    void Application::Run() {
        while (app_running) {
            // TODO: Create new entities

            // Run user logic
            GameLoop();

            RunModules();
            RunSystems();

            app_window->RenderFrame();
        }
    }

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

    void Application::RunModules() {
        for (auto& module : _modules) {
            module->Update();
        }
    }
    void Application::RunSystems() {
        for (auto& system : _systems) {
            system->Update();
        }
    }
}  // namespace Presto