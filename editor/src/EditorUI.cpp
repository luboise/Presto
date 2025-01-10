#include "Presto/Runtime/Window.h"

#include "EditorUI.h"

#include <utility>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "imgui_internal.h"

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

#define TREE_NODE(Label, ...)                                            \
    {                                                                    \
        char label[] = #Label;                                           \
        if (ImGui::TreeNodeEx(                                           \
                label,                                                   \
                flags | (selected == label ? ImGuiTreeNodeFlags_Selected \
                                           : ImGuiTreeNodeFlags_None),   \
                label)) {                                                \
            if (ImGui::IsItemClicked()) {                                \
                selected = label;                                        \
            }                                                            \
            __VA_ARGS__;                                                 \
            ImGui::TreePop();                                            \
        }                                                                \
    }

/*
#define CHILD(Label, x_in, y_in, body)                                  \
    {                                                                   \
        ImVec2 proportions = ImGui::GetContentRegionAvail();            \
        if (ImGui::BeginChild(                                          \
                #Label,                                                 \
                ImVec2(proportions.x * (x_in), proportions.y * (y_in)), \
                ImGuiChildFlags_None)) {                                \
            {body} ImGui::EndChild();                                   \
        }                                                               \
    }
        */

#define CHILD(Label, x_in, y_in, body)                       \
    {                                                        \
        ImVec2 proportions = ImGui::GetContentRegionAvail(); \
        if (ImGui::Begin(#Label)) {                          \
            body ImGui::End();                               \
        }                                                    \
    }

void EditorUI::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::NewFrame();

    auto main_viewport = ImGui::GetMainViewport();

    auto dock_flags{ImGuiDockNodeFlags_PassthruCentralNode |
                    ImGuiDockNodeFlags_NoDockingInCentralNode |
                    ImGuiDockNodeFlags_NoDockingOverCentralNode};

    // Create a DockSpace node where any window can be docked
    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpaceOverViewport(dockspace_id, main_viewport, dock_flags,
                                 nullptr);

    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, dock_flags);

        // ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->);
        ImGuiID left_column = ImGui::DockBuilderSplitNode(
            dockspace_id, ImGuiDir_Left, 0.5f, nullptr, &dockspace_id);
        ImGuiID asset_selector = ImGui::DockBuilderSplitNode(
            dockspace_id, ImGuiDir_Right, 0.5f, nullptr, &dockspace_id);

        ImGui::DockBuilderDockWindow("Left Column", left_column);
        ImGui::DockBuilderDockWindow("Right Column", asset_selector);
        // ImGui::DockBuilderDockWindow("Central Node", dockspace_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    /*
ImGui::SetNextWindowDockID(
    dockspace_id, redock_all ? ImGuiCond_Always : ImGuiCond_FirstUseEver);
ImGuiWindowFlags window_flags =
    (doc->Dirty ? ImGuiWindowFlags_UnsavedDocument : 0);
bool visible = ImGui::Begin(doc->Name, &doc->Open, window_flags);

app.DisplayDocContextMenu(doc);
if (visible) app.DisplayDocContents(doc);

    */

    CHILD(Scene, 0.22, 1, {
        ImGui::Text("Scene");

        // ImGuiTreeNodeFlags
        // flags{ImGuiTreeNodeFlags_CollapsingHeader};
        ImGuiTreeNodeFlags flags{ImGuiTreeNodeFlags_DefaultOpen |
                                 ImGuiTreeNodeFlags_Leaf};

        static std::string selected{};

        TREE_NODE(Main Entity, TREE_NODE(SubEntity))

        ImGui::Separator();

        CHILD(Assets, 1, 0.5, {
            ImGui::Text("Assets");

            for (int i = 0; i < 100; i++) {
                ImGui::Text("%04d: scrollable region", i);
            }
        });
    });

    ImGui::SameLine();

    CHILD(Options, 0.3, 1, {
        ImGui::Text("Options");
        for (int i = 0; i < 100; i++) ImGui::Text("%04d: scrollable region", i);
    });

    ImGui::SameLine();

    ImGui::SetNextWindowDockID(dockspace_id);

    if (ImGui::Begin("Preview", nullptr,
                     ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoBackground)) {
        ImGui::Text("Preview");
        ImGui ::End();
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Masterpiece", "CTRL+SHIFT+N")) {
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
/*
if (ImGui::BeginChild("Scene",
                      ImVec2(region.x * 0.22, region.y * 0.5),
                      ImGuiChildFlags_None)) {

    ImGui::Text("Scene");

    // ImGuiTreeNodeFlags
    // flags{ImGuiTreeNodeFlags_CollapsingHeader};
    ImGuiTreeNodeFlags flags{ImGuiTreeNodeFlags_DefaultOpen |
                             ImGuiTreeNodeFlags_Leaf};

    static std::string selected{};

    TREE_NODE(Main Entity, TREE_NODE(SubEntity))

if (ImGui::TreeNodeEx("Player",
          flags | ImGuiTreeNodeFlags_Selected,
          "Player")) {
if (ImGui::IsItemClicked()) {
node_clicked = i;
}
if (ImGui::TreeNodeEx("Player Head", flags, "Player Head")) {
ImGui::TreePop();
}
ImGui::TreePop();
}
if (ImGui::TreeNodeEx("Alien Enemy", flags, "Alien Enemy")) {
ImGui::TreePop();
};

            ImGui::EndChild();
*/
