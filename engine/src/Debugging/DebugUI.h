#include "Debugging/ComponentBits.h"
#include "Presto/Objects/Scene.h"
#include "Presto/Runtime/Window.h"

#include "ComponentBits.h"

namespace Presto {

class CameraComponent;

class DebugUI {
   public:
    enum class EditorState { EDITING, SELECTING_A_FILE, SELECTING_A_FOLDER };

    static constexpr Presto::scene_name_t NO_SCENE_SELECTED = "NOSCENE";

    static EditorState getEditorState() { return state_; };

    static void initialise(Presto::Window* windowPtr,
                           std::function<void()> exitCallback);

    static void modalPopup(std::string message);
    static void errorPopup(std::string message);

    static void shutdown();

    static void draw();

    static void render();

    static void reloadState();

   private:
    inline static bool visible_{false};

    inline static bool showEntityBrowser_{true};
    inline static bool showComponentBrowser_{true};
    // inline static bool showCameraAdjuster_{true};

    inline static EditorState state_{EditorState::EDITING};
    inline static EntityPtr selectedEntity_{nullptr};

    inline static std::vector<std::string> errorMessages_;

    static void drawMainEditor();

    static void drawEntityBrowser();
    static void drawComponentBrowser();
    static void drawCameraAdjuster();

    static void drawSelectedComponent();

    static void handleInput();

    inline static Ptr<Component> selectedComponent_;

    inline static std::function<void()> exitCallback_;

    inline static CheckedComponentBits componentBits_{-1U};
};
}  // namespace Presto
