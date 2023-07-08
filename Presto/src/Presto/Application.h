#pragma once

#include "prpch.h"

#include "Presto/Core.h"
#include "Presto/Window.h"

namespace Presto {

    class PRESTO_API Application {
       public:
        Application();
        virtual ~Application();

        virtual void Run();

        void OnEvent(Event& e);

       private:
        std::unique_ptr<Window> app_window;
        bool app_running = true;
    };

    // Defined by client (eg. Duality)
    Application* CreateApplication();

}  // namespace Presto