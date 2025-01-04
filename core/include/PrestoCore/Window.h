#pragma once

#include <utility>

#include "Core.h"
#include "PrestoCore/Event.h"

// #include "PrestoCore/Event.h"

constexpr auto DEFAULT_WIDTH = 1280;
constexpr auto DEFAULT_HEIGHT = 720;

namespace Presto {
    struct WindowProperties {
        std::string title;
        unsigned width;
        unsigned height;

        RENDER_LIBRARY render_library = UNSET;

        // Default constructor
        explicit WindowProperties(
            std::string d_title = "Untitled Presto application",
            unsigned d_width = DEFAULT_WIDTH,
            unsigned d_height = DEFAULT_HEIGHT)
            : title(std::move(d_title)), width(d_width), height(d_height) {}
    };

    // Abstracted window interface to be implemented per platform
    class PRESTO_API Window {
        friend class Application;

       public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;
        virtual void Shutdown() = 0;

        // TODO: Move this into the engine
        // virtual void RenderFrame() = 0;

        [[nodiscard]] virtual unsigned GetWidth() const = 0;
        [[nodiscard]] virtual unsigned GetHeight() const = 0;

        virtual void SetCallbackFunction(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool vsync) = 0;
        virtual bool IsVSyncEnabled() = 0;

        virtual void* getWindowPtr() { return _windowPtr; };

        // Window create function that must be implemented per platform
        // Uses default props if unspecified
        static Window* Create(
            const WindowProperties& props = WindowProperties());

       protected:
        void* _windowPtr;
    };

}  // namespace Presto
