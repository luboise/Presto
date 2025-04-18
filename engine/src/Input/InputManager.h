#include "Modules/Module.h"
#include "Presto/Rendering/Renderer.h"

namespace Presto {
class PRESTO_API InputManager : public Module<InputManager> {
   public:
    // Window create function that must be implemented per platform
    // Uses default props if unspecified

    static void init();
    static void shutdown();
    void update() override {}

    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;
    virtual ~InputManager() = delete;

   protected:
    Renderer* _renderer;
};
}  // namespace Presto
