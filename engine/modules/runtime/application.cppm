module;
#include "presto/platform.h"

export module presto.runtime.application;

import std;
import presto.runtime.window;
import presto.core.event;

export namespace Pr {

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
    WindowPtr appWindow_;
    bool running_ = true;

    struct AppOptions {};

    AppOptions options_{};
};

using AppHandle = std::unique_ptr<Pr::Application>;
extern AppHandle createApplication();

}  // namespace Pr
