// #include "prpch.h"

#include "GraphicsModule.h"

namespace Presto {
    Module::MODULE_LAUNCH_CODE GraphicsModule::start() {
        // Initialise the renderer
        return MODULE_LAUNCH_CODE::SUCCESS;
        // Set a graphics library default
    }

    void GraphicsModule::stop() {
        // Destroy the renderer
        return;
    }
}  // namespace Presto
