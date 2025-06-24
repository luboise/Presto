#include <memory>

#include "presto/runtime/application.h"

// #ifdef PR_PLATFORM_WINDOWS

export {
    using AppHandle = std::unique_ptr<Pr::Application>;

    // Pulled from somewhere else

#define PRESTO_ENTRY_POINT()                      \
    int main(int argc, char** argv) {             \
        Pr::Log::init();                          \
        PR_CORE_WARN("Initialised core logger."); \
        PR_INFO("Initialised client logger.");    \
        AppHandle app{Pr::createApplication()};   \
        app->setup();                             \
        app->run();                               \
    };

#define PRESTO_APP_CLASS(ClassName) \
    AppHandle Pr::createApplication() { return std::make_unique<ClassName>(); };
}

// #endif
