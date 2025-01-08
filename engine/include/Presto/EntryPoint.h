#pragma once

// #ifdef PR_PLATFORM_WINDOWS

#include "Presto/Runtime/Application.h"

// Pulled from somewhere else
extern Presto::Application* Presto::CreateApplication();

#define PRESTO_ENTRY_POINT()                      \
    int main(int argc, char** argv) {             \
        Presto::Log::Init();                      \
        PR_CORE_WARN("Initialised core logger."); \
        PR_INFO("Initialised client logger.");    \
        auto* app = Presto::CreateApplication();  \
        app->setup();                             \
        app->run();                               \
        delete app;                               \
    };

// #endif
