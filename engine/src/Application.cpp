#include "Presto/Runtime/Application.h"

#include "Managers.h"

#include "Presto/Events.h"
#include "Presto/Objects/Components/CameraComponent.h"
#include "Presto/Runtime.h"

#include "Utils/DebugTimer.h"

#ifdef PR_DEBUG_BUILD
#include "Debugging/DebugUI.h"
#endif

namespace Presto {

Application::Application() {
    // TODO: Fix this to be injected
    this->appWindow_ = Window::create();
    // this->_app_window = new GLFWAppWindow();
    appWindow_->setCallbackFunction(
        [this](auto& e) -> void { this->onEvent(e); });

    AssetManager::init();
    // AssetManager must be initialised before RenderingManager

    RenderingManager::setRenderLibrary(OPENGL);

    GLFWAppWindow* window{dynamic_cast<GLFWAppWindow*>(appWindow_.get())};

    RenderingManager::setWindow(window);
    RenderingManager::setWindow(window);

    EntityManagerImpl::init();

    RenderingManager::init();

    // SceneManager::init();
    EventManager::init();
    PhysicsManager::init();
    Time::init();

    PR_DEBUG_ONLY_CODE(DebugUI::initialise(window, [this] { this->exit(); }))
}

Application::~Application() { /*this->app_window->Shutdown();*/
    PR_DEBUG_ONLY_CODE(DebugUI::shutdown())

    PhysicsManager::shutdown();
    EventManager::shutdown();
    // SceneManager::shutdown();
    AssetManager::shutdown();
    EntityManagerImpl::shutdown();
    RenderingManager::shutdown();

    this->appWindow_->shutdown();
    // this->_app_window.release();
};

void Application::run() {
    DebugTimer game_loop_timer("User Game Loop");
    DebugTimer window_timer("Updating GLFWWindow");
    DebugTimer rendering_timer("Rendering All Entities");
    DebugTimer time_update("Global Time Update");
    DebugTimer garbage_collection_timer("Garbage Collection");

    RenderingManager& rendering = RenderingManager::get();
    EntityManagerImpl& entities = EntityManagerImpl::get();
    PhysicsManager& physics = PhysicsManager::get();

    while (running_) {
        // Calculate delta

        // PRINT FPS
        // PR_CORE_TRACE("{:.2f} FPS", 1 / Time::deltaSeconds());

        // time_update.reset();
        Time::update();
        // time_update.printElapsed();

        entities.instantiateEntities();

        // Draw the debug UI
        PR_DEBUG_ONLY_CODE(DebugUI::draw())

        // USER PRE-LOOP LOGIC
        preLoop();

        // USER GAME-LOOP LOGIC
        gameLoop();
        // game_loop_timer.printElapsed();

        // rendering_timer.reset();
        // runSystems();
        // rendering_timer.printElapsed();

        // window_timer.reset();
        appWindow_->update();
        // window_timer.printElapsed();

        entities.update();
        physics.update();
        rendering.update();

        // rendering_timer.printElapsed();

        PR_DEBUG_ONLY_CODE(DebugUI::render())

        // USER POST-LOOP LOGIC
        postLoop();

        rendering.clear();

        // garbage_collection_timer.reset();
        entities.collectGarbage();
        // garbage_collection_timer.printElapsed();
    }

    // User teardown function
    tearDown();
}

void Application::onEvent(Event& e) {
    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<WindowResizeEvent>(
        [this](auto& e) -> bool { return this->onWindowResize(e); });

    dispatcher.Dispatch<WindowCloseEvent>(
        [this](auto& e) -> bool { return this->onWindowClose(e); });

    dispatcher.Dispatch<FramebufferResizedEvent>([](auto& /*e*/) -> bool {
        RenderingManager::get().resizeFramebuffer();
        return true;
    });

    // TODO: Properly put this into the event system
    auto lambda{[](KeyEvent& event) -> bool {
        EventManager::get().onKeyEvent(event);
        return true;
    }};

    if (e.inCategory(EventCategoryKeyboard)) {
        if (auto* ptr = dynamic_cast<KeyEvent*>(&e); ptr != nullptr) {
            lambda(*ptr);
        }
    }

    // TODO: Implement log levels
    // PR_CORE_TRACE("{}", e.toString());
}

// TODO: Implement this
bool Application::onWindowResize(WindowResizeEvent& /*e*/) { return true; };

bool Application::onWindowClose(WindowCloseEvent& /*e*/) {
    this->running_ = false;
    return true;
}

/*
void Application::runSystems() {
for (auto& system : _systems) {
    system->Update();
}
}
*/

Window& Application::getWindow() const { return *appWindow_; };
}  // namespace Presto
