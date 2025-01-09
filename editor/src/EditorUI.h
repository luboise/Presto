#include "Presto/Runtime/Window.h"

class EditorUI {
   public:
    enum class EditorState { EDITING, SELECTING_A_FILE, SELECTING_A_FOLDER };

    static EditorState getEditorState() { return state_; };

    static void initialise(Presto::Window* windowPtr,
                           std::function<void()> exitCallback);

    static void tearDown();

    static void draw();
    static void render();

   private:
    inline static EditorState state_{EditorState::EDITING};

    inline static std::function<void()> exitCallback_;
};
