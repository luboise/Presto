#pragma once

#include "Presto/Core.h"
#include "prpch.h"

namespace Presto {

    struct WindowProperties {
        std::string title;
        unsigned height;
        unsigned width;

        // Default constructor
        WindowProperties(
            const std::string& title = "Untitled Presto application",
            unsigned width = 1280, unsigned height = 720)
            : title(title), width(width), height(height) {}
    }

    // Abstracted window interface to be implemented per platform
    class PRESTO_API Window {
       public:
        using CallbackFunction = std::function<void(Event&)>;

        virtual ~Window();

        virtual void OnUpdate() = 0;

        virtual unsigned GetWidth() const = 0;
        virtual unsigned GetHeight() const = 0;

        virtual void SetCallbackFunction(const CallbackFunction& callback) = 0;
        virtual void SetVSync(bool vsync) = 0;
        virtual bool IsVSyncEnabled() = 0;

        static Window* Create(const WindowsProps& props = WindowProps());
    };

}  // namespace Presto