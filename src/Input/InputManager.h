#include "Presto/Module.h"
#include "Presto/Rendering/Renderer.h"

namespace Presto {
    class PRESTO_API InputManager : public Module {
       public:
        virtual ~InputManager() {};
        virtual void Shutdown() = 0;

        virtual void OnUpdate() = 0;

        virtual unsigned GetWidth() const = 0;
        virtual unsigned GetHeight() const = 0;

        virtual void SetCallbackFunction(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool vsync) = 0;
        virtual bool IsVSyncEnabled() = 0;

        // Window create function that must be implemented per platform
        // Uses default props if unspecified
        static InputManager* Create();

       protected:
        Renderer* _renderer;
    };
}  // namespace Presto
