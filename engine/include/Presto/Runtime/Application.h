#pragma once

#include "Presto/Runtime/Window.h"

namespace Presto {
class Event;

class WindowCloseEvent;
class WindowResizeEvent;

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

    void exit() { running_ = false; };

    [[nodiscard]] Window& getWindow() const;

    // Intended to be replaced by user logic
    virtual void gameLoop() {};

    void initialiseEvents();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

   private:
    bool onWindowClose(WindowCloseEvent& e);
    bool onWindowResize(WindowResizeEvent& e);

    // void runSystems();

    WindowPtr appWindow_;
    bool running_ = true;

    // std::vector<Module*> _modules;
    // std::vector<System*> _systems;

    struct AppOptions {};

    AppOptions options_{};
};

using AppHandle = std::unique_ptr<Presto::Application>;
extern AppHandle createApplication();

}  // namespace Presto
