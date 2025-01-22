#include "Presto/Runtime/Application.h"
#include "Presto/Modules/EntityManager.h"
#include "Presto/Modules/EventManager.h"
#include "Presto/Modules/SceneManager.h"

#include "Presto/Runtime.h"

#include "Presto/Managers.h"

#include "Presto/Runtime/Events/ApplicationEvents.h"

#include "Presto/Runtime/Events/Event.h"
#include "Presto/Runtime/GLFWAppWindow.h"

#include "Utils/DebugTimer.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Presto {
    Application::Application() {
        // TODO: Fix this to be injected
        auto* app_window = (dynamic_cast<GLFWAppWindow*>(Window::Create()));

        this->_app_window = std::unique_ptr<GLFWAppWindow>(app_window);
        // this->_app_window = new GLFWAppWindow();
        _app_window->SetCallbackFunction(BIND_EVENT_FN(Application::OnEvent));

        RenderingManager::setRenderLibrary(OPENGL);
        RenderingManager::setWindow(app_window);
        RenderingManager::setWindow(app_window);

        RenderingManager::init();
        EntityManager::init();
        ResourceManager::init();
        SceneManager::init();
        EventManager::init();
        Time::init();
    }

    Application::~Application() { /*this->app_window->Shutdown();*/
        EventManager::shutdown();
        SceneManager::shutdown();
        ResourceManager::shutdown();
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

        RenderingManager& rm = RenderingManager::get();
        EntityManager& em = EntityManager::get();

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

            em.update();

            rm.update();
            // rendering_timer.printElapsed();

            postLoop();

            rm.clear();

            // garbage_collection_timer.reset();
            em.collectGarbage();
            // garbage_collection_timer.printElapsed();
        }

        // User teardown function
        tearDown();
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(
            BIND_EVENT_FN(Application::OnWindowClose));

        // TODO: Properly put this into the event system
        auto lambda{[](KeyEvent& event) -> bool {
            EventManager::get().onKeyEvent(event);
            return true;
        }};

        if (e.IsInCategory(EventCategoryKeyboard)) {
            if (auto* ptr = dynamic_cast<KeyEvent*>(&e); ptr != nullptr) {
                lambda(*ptr);
            }
        }

        // TODO: Implement log levels
        // PR_CORE_TRACE("{}", e.ToString());
    }

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
