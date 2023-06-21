#pragma once

#ifdef PR_PLATFORM_WINDOWS

// Pulled from somewhere else
extern Presto::Application* Presto::CreateApplication();

int main(int argc, char** argv) {
    auto app = Presto::CreateApplication();
    app->Run();
    delete app;
}

#endif