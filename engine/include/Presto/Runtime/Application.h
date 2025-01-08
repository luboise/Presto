#pragma once

// #include "Presto/Events/ApplicationEvents.h"
// #include "Presto/Window.h"

#include "Events/ApplicationEvents.h"
#include "Events/Event.h"
#include "Window.h"

#include "Presto/Objects/System.h"

namespace Presto {
    class Camera;

    class PRESTO_API Application {
       public:
        Application();
        virtual ~Application();

        virtual void run();

        // These are overridden by user implementations
        virtual void preLoop() {};
        virtual void postLoop() {};
        virtual void setup() {};
        virtual void tearDown() {};

        [[nodiscard]] Window* GetWindow() const { return _app_window.get(); };

        // Intended to be replaced by user logic
        virtual void GameLoop() {};
        Camera& GetMainCamera();

        void OnEvent(Event& e);

       private:
        bool OnWindowClose(WindowCloseEvent& e);

        void RunSystems();

        std::unique_ptr<Window> _app_window;
        bool app_running = true;

        // std::vector<Module*> _modules;
        std::vector<System*> _systems;

       protected:
        Camera* _mainCamera;
        double _delta = 0;
        double _currentTime = 0;
    };

    // Defined by client (eg. Duality)
    Application* CreateApplication();

}  // namespace Presto
