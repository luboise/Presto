#include "Presto/Module.h"
#include "Presto/Rendering/Renderer.h"

namespace Presto {
    class PRESTO_API InputManager : public Module {
       public:
        virtual ~InputManager() {};

        // Window create function that must be implemented per platform
        // Uses default props if unspecified
        static InputManager* Create();

       protected:
        Renderer* _renderer;
    };
}  // namespace Presto
