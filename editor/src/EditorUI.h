#include "Presto/Objects/Scene.h"
#include "Presto/Runtime/Window.h"

class EditorUI {
   public:
    enum class EditorState { EDITING, SELECTING_A_FILE, SELECTING_A_FOLDER };

    static constexpr Presto::scene_name_t NO_SCENE_SELECTED = "NOSCENE";

    static EditorState getEditorState() { return state_; };

    static void initialise(Presto::Window* windowPtr,
                           std::function<void()> exitCallback);

    static void modalPopup(std::string message);
    static void errorPopup(std::string message);

    static void tearDown();

    static void draw();

    static void render();

    static void reloadState();

   private:
    inline static EditorState state_{EditorState::EDITING};
    inline static Presto::scene_name_t selectedScene_{NO_SCENE_SELECTED};

    inline static std::vector<std::string> errorMessages_;

    static void drawMainEditor();

    inline static std::function<void()> exitCallback_;
};
