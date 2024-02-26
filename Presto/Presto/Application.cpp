
#include "Application.h"
#include "Modules/ObjectsModule/EntityManager.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Presto {
    Application::Application() {
        app_window = std::unique_ptr<Window>(Window::Create());
        app_window->SetCallbackFunction(BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application() { /*this->app_window->Shutdown();*/
        this->app_window->Shutdown();
        this->app_window.release();
    };

    void Application::Run() {
        while (app_running) {
            // Create new entities

            // Run user logic
            GameLoop();

            // Run modules
            RunModules();

            // Run systems

            for (auto& system : _systems) {
                system.Update();
            }

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
            module.Update();
        }
    }
}  // namespace Presto