#include "Presto/Runtime/Application.h"

#include "Presto/Managers.h"

#include "Presto/Components/CameraComponent.h"

#include "Presto/Runtime.h"

#include "Presto/Modules/SceneManager.h"

#include "Modules/EventManager.h"

#include "Presto/Events.h"

#include "Presto/Runtime/GLFWAppWindow.h"

#include "Utils/DebugTimer.h"

namespace Presto {
    Application::Application() {
        // TODO: Fix this to be injected
        auto* app_window = (dynamic_cast<GLFWAppWindow*>(Window::create()));

        this->_app_window = std::unique_ptr<GLFWAppWindow>(app_window);
        // this->_app_window = new GLFWAppWindow();
        _app_window->setCallbackFunction(
            [this](auto& e) -> void { this->onEvent(e); });

        RenderingManager::setRenderLibrary(OPENGL);
        RenderingManager::setWindow(app_window);
        RenderingManager::setWindow(app_window);

        EntityManager::init();

        CameraComponent* default_camera{
            EntityManager::get().newComponent<CameraComponent>()};
        RenderingManager::init(*default_camera);

        AssetManager::init();
        SceneManager::init();
        EventManager::init();
        PhysicsManager::init();
        Time::init();
    }

    Application::~Application() { /*this->app_window->Shutdown();*/
        PhysicsManager::shutdown();
        EventManager::shutdown();
        SceneManager::shutdown();
        AssetManager::shutdown();
        EntityManager::shutdown();
        RenderingManager::shutdown();

        this->_app_window->shutdown();
        // this->_app_window.release();
    };

    void Application::run() {
        DebugTimer game_loop_timer("User Game Loop");
        DebugTimer window_timer("Updating GLFWWindow");
        DebugTimer rendering_timer("Rendering All Entities");
        DebugTimer time_update("Global Time Update");
        DebugTimer garbage_collection_timer("Garbage Collection");

        RenderingManager& rendering = RenderingManager::get();
        EntityManager& entities = EntityManager::get();
        PhysicsManager& physics = PhysicsManager::get();

        while (app_running) {
            // Calculate delta

            // PRINT FPS
            // PR_CORE_TRACE("{:.2f} FPS", 1 / Time::deltaSeconds());

            // TODO: Create new entities at start of new frame

            // time_update.reset();
            Time::update();
            // time_update.printElapsed();

            preLoop();

            // Run user logic
            // game_loop_timer.reset();
            gameLoop();
            // game_loop_timer.printElapsed();

            // rendering_timer.reset();
            RunSystems();
            // rendering_timer.printElapsed();

            // window_timer.reset();
            _app_window->update();
            // window_timer.printElapsed();

            entities.update();
            physics.update();
            rendering.update();

            // rendering_timer.printElapsed();

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
            [this](auto& e) -> bool { return this->OnWindowClose(e); });

        dispatcher.Dispatch<FramebufferResizedEvent>([](auto& e) -> bool {
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

    bool Application::onWindowResize(WindowResizeEvent& e) { return true; };

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        this->app_running = false;
        return true;
    }

    void Application::RunSystems() {
        for (auto& system : _systems) {
            system->Update();
        }
    }
}  // namespace Presto
