#pragma once

#include "Events/Event.h"

#include "Presto/Core/Types.h"

// #include "Presto/Event.h"

constexpr auto DEFAULT_WIDTH = 2560;
constexpr auto DEFAULT_HEIGHT = 1440;

namespace Presto {
    struct WindowProperties {
        std::string title{"Untitled Presto application"};
        VisualExtents extents{DEFAULT_WIDTH, DEFAULT_HEIGHT};
        RENDER_LIBRARY render_library{UNSET};
    };

    // Abstracted window interface to be implemented per platform
    class Window {
        friend class Application;

       public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;
        virtual void shutdown() = 0;

        // TODO: Move this into the engine
        virtual void update() = 0;

        [[nodiscard]] virtual unsigned getWidth() const = 0;
        [[nodiscard]] virtual unsigned getHeight() const = 0;

        virtual void setCallbackFunction(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool vsync) = 0;
        virtual bool vSyncEnabled() = 0;

        virtual void* getWindowPtr() { return windowPtr_; };

        // Window create function that must be implemented per platform
        // Uses default props if unspecified
        static Window* create(
            const WindowProperties& props = WindowProperties());

       protected:
        void* windowPtr_;
    };

}  // namespace Presto
