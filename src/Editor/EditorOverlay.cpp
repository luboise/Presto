#include "EditorOverlay.h"
#include "backends/imgui_impl_glfw.h"

#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

namespace Presto {
    GLFWAppWindow* EditorOverlay::_window = nullptr;

    void EditorOverlay::Init(GLFWAppWindow* window) {
        EditorOverlay::_window = window;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(window->getWindowHandle(), true);
        ImGui_ImplOpenGL3_Init();
    };

    void EditorOverlay::OnLoopStart() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::EndFrame();
    }

    void EditorOverlay::OnLoopEnd() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}  // namespace Presto
