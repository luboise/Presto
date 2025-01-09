#include "Presto/Runtime/Window.h"

#include "EditorUI.h"

#include <utility>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

void EditorUI::initialise(Presto::Window* windowPtr,
                          std::function<void()> exitCallback) {
    EditorUI::exitCallback_ = std::move(exitCallback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    auto* window{static_cast<GLFWwindow*>(windowPtr->getWindowPtr())};

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window), true);

    ImGui_ImplOpenGL3_Init();
};

void EditorUI::tearDown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EditorUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool use_work_area = true;
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBackground;

    // We demonstrate using the full viewport area or the work area
    // (without menu-bars, task-bars etc.) Based on your use case you
    // may want one or the other.
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize
                                           : viewport->Size);

    if (ImGui::Begin("Example: Fullscreen window", nullptr, flags)) {
        if (ImGui::Button("Close this window")) {
            EditorUI::exitCallback_();
        }

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Masterpiece", "CTRL+SHIFT+N")) {
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        /*
HelpMarker(
"Main Area = entire viewport,\nWork Area = entire viewport "
"minus sections used by the main menu bars, task bars "
"etc.\n\nEnable the main-menu bar in Examples menu to see "
"the difference.");
                */
    }
    ImGui::End();
}
