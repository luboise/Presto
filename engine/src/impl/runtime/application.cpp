module presto.runtime.application;

import presto.core.event;
import presto.core.platform;
import presto.internal.managers;

#ifdef PR_DEBUG_BUILD
import presto.internal.managers.debug;
#endif

namespace Presto {

Application::Application() {
    // TODO: Fix this to be injected
    this->appWindow_ = Window::create();

    EventManagerImpl::init();
    this->initialiseEvents();  // Initialise application events in EventManager

    // this->_app_window = new GLFWAppWindow();
    AssetManager::init();
    // AssetManager must be initialised before RenderingManager

    RenderingManager::setRenderLibrary(OPENGL);

    GLFWAppWindow* window{dynamic_cast<GLFWAppWindow*>(appWindow_.get())};

    RenderingManager::setWindow(window);
    RenderingManager::setWindow(window);

    EntityManagerImpl::init();

    RenderingManager::init();
    RenderingManager::get().loadDefaults();

    // SceneManager::init();
    PhysicsManager::init();
    Time::init();

#ifdef PR_DEBUG_BUILD
    DebugManager::init(window, [this] { this->exit(); });
#endif
}

Application::~Application() { /*this->app_window->Shutdown();*/
    PR_DEBUG_ONLY_CODE(DebugManager::shutdown());

    PhysicsManager::shutdown();
    EventManagerImpl::shutdown();
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

    PR_DEBUG_ONLY_CODE(DebugManager& debugging = DebugManager::get())

    while (running_) {
        // Calculate delta

        // PRINT FPS
        // PR_CORE_TRACE("{:.2f} FPS", 1 / Time::deltaSeconds());

        // time_update.reset();
        Time::update();
        // time_update.printElapsed();

        entities.instantiateEntities();

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

        // USER POST-LOOP LOGIC
        postLoop();

        // TODO: Check if this is still needed
        PR_DEBUG_ONLY_CODE(debugging.drawAll())

        PR_DEBUG_ONLY_CODE(debugging.update());

        rendering.clear();

        // garbage_collection_timer.reset();
        entities.collectGarbage();
        // garbage_collection_timer.printElapsed();
    }

    // User teardown function
    tearDown();
}

void Application::initialiseEvents() {
    auto& em{EventManagerImpl::get()};

    em.addHandler<WindowResizeEvent>(
        [this](WindowResizeEvent& e) { this->onWindowResize(e); });

    em.addHandler<WindowCloseEvent>(
        [this](auto& e) -> bool { return this->onWindowClose(e); });
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
