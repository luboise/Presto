#pragma once

#include "Core.h"
#include "Presto/Events/ApplicationEvents.h"
#include "Presto/Window.h"

#include "Presto/Module.h"
#include "Presto/Objects/System.h"
#include "Rendering/Camera.h"

namespace Presto {
    class PRESTO_API Application {
       public:
        Application();
        virtual ~Application();

        virtual void Setup() {};

        virtual void Run();

        // Intended to be replaced by user logic
        virtual void GameLoop() {};
        Camera& GetMainCamera();

        void OnEvent(Event& e);

       private:
        bool OnWindowClose(WindowCloseEvent& e);

        void RunSystems();

        std::unique_ptr<Window> app_window;
        bool app_running = true;

        std::vector<Module*> _modules;
        std::vector<System*> _systems;

       protected:
        Camera _mainCamera;
        double _delta = 0;
        double _currentTime = 0;
    };

    // Defined by client (eg. Duality)
    Application* CreateApplication();

}  // namespace Presto
