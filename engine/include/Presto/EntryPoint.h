#pragma once

// #ifdef PR_PLATFORM_WINDOWS

#include "Presto/Runtime/Application.h"

using AppHandle = std::unique_ptr<Presto::Application>;

// Pulled from somewhere else

#define PRESTO_ENTRY_POINT()                        \
    int main(int argc, char** argv) {               \
        Presto::Log::Init();                        \
        PR_CORE_WARN("Initialised core logger.");   \
        PR_INFO("Initialised client logger.");      \
        AppHandle app{Presto::createApplication()}; \
        app->setup();                               \
        app->run();                                 \
    };

#define PRESTO_APP_CLASS(ClassName)           \
    AppHandle Presto::createApplication() {   \
        return std::make_unique<ClassName>(); \
    };

// #endif
