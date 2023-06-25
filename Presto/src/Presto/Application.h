#pragma once

#include "Presto/Core.h"

namespace Presto {

    class PRESTO_API Application {
       public:
        Application();
        virtual ~Application();

        virtual void Run();
    };

    // Defined by client (eg. Duality)
    Application* CreateApplication();

}  // namespace Presto