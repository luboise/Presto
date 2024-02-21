#pragma once

#include "Presto/Core.h"
#include "Presto/Events/ApplicationEvents.h"
#include "Presto/Window.h"

#include "Modules/ObjectsModule/System/System.h"

#include <memory>

namespace Presto {

    class PRESTO_API Application {
       public:
        Application();
        virtual ~Application();

        virtual void Run();

        // Intended to be replaced by user logic
        virtual void GameLoop(){};

        void OnEvent(Event& e);

       private:
        bool OnWindowClose(WindowCloseEvent& e);
        void RunModules();

        std::unique_ptr<Window> app_window;
        bool app_running = true;

        std::vector<Module> _modules;
        std::vector<System> _systems;
    };

    // Defined by client (eg. Duality)
    Application* CreateApplication();

}  // namespace Presto