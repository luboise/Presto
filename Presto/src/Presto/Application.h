#pragma once

#include "Core.h"

namespace Presto {

class PRESTO_API Application {
   public:
    Application();
    virtual ~Application();

    void Run();  
};

// Defined by client (eg. Duality)
Application* CreateApplication();

}  // namespace Presto