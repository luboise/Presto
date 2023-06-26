#include "prpch.h"

#include "Application.h"

namespace Presto {
    Application::Application() {
        // app_window = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application(){};

    void Application::Run() {
        while (app_running) {
            /*app_window->OnUpdate();*/
        }
    }
}  // namespace Presto