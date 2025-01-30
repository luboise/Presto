#include "Presto/Objects/Scene.h"
#include "Presto/Runtime/Window.h"

namespace Presto {
    class DebugUI {
       public:
        enum class EditorState {
            EDITING,
            SELECTING_A_FILE,
            SELECTING_A_FOLDER
        };

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

        inline static EditorState state_{EditorState::EDITING};
        inline static entity_ptr selectedEntity_{nullptr};

        inline static std::vector<std::string> errorMessages_;

        static void drawMainEditor();

        static void handleInput();

        inline static std::function<void()> exitCallback_;
    };
}  // namespace Presto
