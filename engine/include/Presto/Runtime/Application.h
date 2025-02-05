#pragma once

// #include "Presto/Events/ApplicationEvents.h"
// #include "Presto/Window.h"

#include "Presto/Runtime/Events/ApplicationEvents.h"
#include "Window.h"

#include "Presto/Objects/System.h"

namespace Presto {
    class Event;
    class WindowCloseEvent;

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

        void exit() { app_running = false; };

        [[nodiscard]] Window* GetWindow() const { return _app_window.get(); };

        // Intended to be replaced by user logic
        virtual void gameLoop() {};

        void onEvent(Event& e);

       private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

        void RunSystems();

        std::unique_ptr<Window> _app_window;
        bool app_running = true;

        // std::vector<Module*> _modules;
        std::vector<System*> _systems;

        struct AppOptions {};

        AppOptions options_{};
    };

    using AppHandle = std::unique_ptr<Presto::Application>;
    extern AppHandle createApplication();

}  // namespace Presto
