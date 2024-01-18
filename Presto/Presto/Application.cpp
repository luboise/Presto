// #include "prpch.h"

#include "Application.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Presto {
    Application::Application() {
        app_window = std::unique_ptr<Window>(Window::Create());
        app_window->SetCallbackFunction(BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application(){};

    void Application::Run() {
        while (app_running) {
            app_window->OnUpdate();
        }

        this->app_window->Shutdown();
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
}  // namespace Presto