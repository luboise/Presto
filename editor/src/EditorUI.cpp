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
        /*
if (ImGui::Button("Close this window")) {
    EditorUI::exitCallback_();
}
        */

        /*
        if (ImGui::BeginTable("Main Table", 3)) {
            ImGui::TableSetupColumn("Column 1",
                                    ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Column 2",
                                    ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Column 3",
                                    ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImVec2 fullSize = ImGui::GetContentRegionAvail();

            ImGui::SetNextWindowSize(fullSize);

            if (ImGui::Begin("Pog assets", nullptr, 0)) {
                ImGui::Text("bruh");
                ImGui::End();
            };

            ImGui::Text("heyy");
            ImGui::TableNextColumn();
            ImGui::Text("heyy2");

            ImGui::EndTable();
        }

        */

        ImVec2 region = ImGui::GetContentRegionAvail();

        {
            ImGui::BeginChild("Scene", ImVec2(region.x * 0.22, region.y * 0.5),
                              ImGuiChildFlags_None);

            ImGui::Text("Scene");

            // ImGuiTreeNodeFlags flags{ImGuiTreeNodeFlags_CollapsingHeader};
            ImGuiTreeNodeFlags flags{ImGuiTreeNodeFlags_DefaultOpen |
                                     ImGuiTreeNodeFlags_Leaf};

            static std::string selected{};
            PR_CORE_TRACE(selected);

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

            TREE_NODE(Main Entity, TREE_NODE(SubEntity))

            /*
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
        */

            ImGui::EndChild();
        }

        /*
                {
                    ImGui::BeginChild("Assets", ImVec2(region.x * 0.33, region.y
           * 0.5), ImGuiChildFlags_None);

                    ImGui::Text("Assets");

                    for (int i = 0; i < 100; i++)
                        ImGui::Text("%04d: scrollable region", i);
                    ImGui::EndChild();
                }
                        */

        {
            ImGui::SameLine();
            ImGui::BeginChild("Options", ImVec2(region.x * 0.22, region.y),
                              ImGuiChildFlags_None);

            ImGui::Text("Options");
            for (int i = 0; i < 100; i++)
                ImGui::Text("%04d: scrollable region", i);
            ImGui::EndChild();
        }
        {
            ImGui::SameLine();
            ImGui::BeginChild("Preview", ImGui::GetContentRegionAvail(),
                              ImGuiChildFlags_None);

            ImGui::Text("Preview");

            /*
        for (int i = 0; i < 100; i++)
        ImGui::Text("%04d: scrollable region", i);
            */
            ImGui::EndChild();
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
    HelpMarker(
    "Main Area = entire viewport,\nWork Area = entire viewport "
    "minus sections used by the main menu bars, task bars "
    "etc.\n\nEnable the main-menu bar in Examples menu to see "
    "the difference.");
            */

    ImGui::End();
}
