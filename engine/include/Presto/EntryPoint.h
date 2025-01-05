#pragma once

// #ifdef PR_PLATFORM_WINDOWS

#include "Application.h"  // IWYU pragma: export

// Pulled from somewhere else
extern Presto::Application* Presto::CreateApplication();

int main(int argc, char** argv) {
    Presto::Log::Init();
    PR_CORE_WARN("Initialised core logger.");
    PR_INFO("Initialised client logger.");

    auto* app = Presto::CreateApplication();
    app->Setup();
    app->Run();
    delete app;
}

// #endif
