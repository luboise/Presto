export module presto.internal.debugging;

export import :constants;
import presto.core.types;

export namespace Pr {

class CameraComponent;
class RenderingManager;

class DebugCameraListener {
   public:
    DebugCameraListener();
    explicit DebugCameraListener(RenderingManager&);
    void toggle();

    [[nodiscard]] bool enabled() const;

    CameraComponent& camera();

   private:
    bool enabled_{false};
    Ptr<CameraComponent> camera_;
};

class DebugUI {
   public:
    enum class EditorState { EDITING, SELECTING_A_FILE, SELECTING_A_FOLDER };

    static constexpr Pr::scene_name_t NO_SCENE_SELECTED = "NOSCENE";

    static EditorState getEditorState() { return state_; };

    static void initialise(Pr::Window* windowPtr,
                           std::function<void()> exitCallback);

    static void modalPopup(Pr::string message);
    static void errorPopup(Pr::string message);

    static void shutdown();

    static void draw();

    static void render();

    static void reloadState();

   private:
    inline static DebugCameraListener debugCamera_;

    inline static bool visible_{false};

    inline static EditorState state_{EditorState::EDITING};
    inline static EntityPtr selectedEntity_{nullptr};

    inline static std::vector<Pr::string> errorMessages_;

    static void drawMainEditor();

    inline static bool showEntityBrowser_{true};
    static void drawEntityBrowser();

    inline static bool showComponentBrowser_{true};
    static void drawComponentBrowser();

    inline static bool showCameraBrowser_{true};
    static void drawCameraBrowser();

    static void drawCameraModifier(Ptr<CameraComponent>);
    static void drawCameraModifier(CameraComponent&);

    static void drawSelectedComponent();

    static void handleInput();

    inline static Ptr<Component> selectedComponent_;

    inline static std::function<void()> exitCallback_;

    inline static CheckedComponentBits componentBits_{-1U};
};
}  // namespace Pr
