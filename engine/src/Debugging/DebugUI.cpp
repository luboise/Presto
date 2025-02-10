#include "Debugging/ComponentBits.h"
#include "Presto/Components/Renderable/ModelComponent.h"
#include "Presto/Components/TransformComponent.h"
#include "Presto/Modules/EntityManager.h"
#include "Presto/Objects/Component.h"
#include "Presto/Objects/Entity.h"
#include "Presto/Runtime/Window.h"

#include "DebugUI.h"

#include <utility>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace Presto {

void DebugUI::initialise(Presto::Window *windowPtr,
                         std::function<void()> exitCallback) {
    DebugUI::exitCallback_ = std::move(exitCallback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    auto *window{static_cast<GLFWwindow *>(windowPtr->getWindowPtr())};

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init();
}

void DebugUI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#define TREE_NODE(Label, ...)                                                  \
    {                                                                          \
        char label[] = #Label;                                                 \
        if (ImGui::TreeNodeEx(                                                 \
                label,                                                         \
                flags | (selected_scene == label ? ImGuiTreeNodeFlags_Selected \
                                                 : ImGuiTreeNodeFlags_None),   \
                label)) {                                                      \
            if (ImGui::IsItemClicked()) {                                      \
                selected_scene = label;                                        \
            }                                                                  \
            __VA_ARGS__;                                                       \
            ImGui::TreePop();                                                  \
        }                                                                      \
    }

#define ENTITY_TREE_NODE(Label, ...)                                           \
    {                                                                          \
        std::string label{Label};                                              \
                                                                               \
        if (ImGui::TreeNodeEx(                                                 \
                label.data(),                                                  \
                flags | (selected_scene == label ? ImGuiTreeNodeFlags_Selected \
                                                 : ImGuiTreeNodeFlags_None),   \
                label.data())) {                                               \
            if (ImGui::IsItemClicked()) {                                      \
                selected_scene = label;                                        \
            }                                                                  \
            __VA_ARGS__;                                                       \
            ImGui::TreePop();                                                  \
        }                                                                      \
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

#define CHILD(Label, x_in, y_in, body)                           \
    {                                                            \
        /*ImVec2 proportions = ImGui::GetContentRegionAvail();*/ \
        if (ImGui::Begin(#Label)) {                              \
            body ImGui::End();                                   \
        }                                                        \
    }

void DebugUI::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    DebugUI::handleInput();
    ImGui::NewFrame();

    if (!visible_) {
        return;
    }

    if (errorMessages_.size() > 0) {
        if (ImGui::Begin("Error box")) {
            ImGui::Text("%s", errorMessages_.front().data());
            if (ImGui::Button("OK")) {
                errorMessages_.erase(errorMessages_.begin());
            }
            ImGui::End();
        }
    } else if (state_ == EditorState::EDITING) {
        drawMainEditor();
    } else if (state_ == EditorState::SELECTING_A_FILE) {
        ImGui::Begin("File Selector");
        static std::array<char, 100> chosen_file;

        ImGui::InputText("Path", chosen_file.data(), 100,
                         ImGuiInputTextFlags_ElideLeft);
        ImGui::SameLine();

        if (ImGui::Button("Open")) {
            fs::path chosen_path{std::string(chosen_file.data())};

            /*
      if (!MasterpieceManager::get().openMasterpiece(chosen_path)) {
      errorMessages_.emplace_back("Unable to open masterpiece.");
      } else {
      reloadState();
      };
            */

            state_ = EditorState::EDITING;
        }

        ImGui::End();
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Masterpiece", "CTRL+SHIFT+N")) {
            } else if (ImGui::MenuItem("Open Masterpiece", "CTRL+O") &&
                       state_ == EditorState::EDITING) {
                state_ = EditorState::SELECTING_A_FILE;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void DebugUI::drawMainEditor() {
    const auto* main_viewport = ImGui::GetMainViewport();

    constexpr auto dock_flags{ImGuiDockNodeFlags_PassthruCentralNode |
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
            dockspace_id, ImGuiDir_Left, 0.5F, nullptr, &dockspace_id);
        ImGuiID asset_selector = ImGui::DockBuilderSplitNode(
            dockspace_id, ImGuiDir_Right, 0.5F, nullptr, &dockspace_id);

        ImGui::DockBuilderGetNode(dockspace_id)->LocalFlags |=
            ImGuiDockNodeFlags_CentralNode;

        ImGui::DockBuilderDockWindow("Left Column", left_column);
        ImGui::DockBuilderDockWindow("Right Column", asset_selector);
        // ImGui::DockBuilderDockWindow("Central Node", dockspace_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    /*
  ImGui::SetNextWindowDockID(
    dockspace_id, redock_all ? ImGuiCond_Always :
  ImGuiCond_FirstUseEver); ImGuiWindowFlags window_flags = (doc->Dirty ?
  ImGuiWindowFlags_UnsavedDocument : 0); bool visible =
  ImGui::Begin(doc->Name, &doc->Open, window_flags);

  app.DisplayDocContextMenu(doc);
  if (visible) app.DisplayDocContents(doc);

    */

    if (showEntityBrowser_) {
        drawEntityBrowser();
    };

    if (showComponentBrowser_) {
        drawComponentBrowser();
    };

    CHILD(Assets, 1, 0.5, {
        ImGui::Text("Assets");

        for (int i = 0; i < 100; i++) {
            ImGui::Text("%04d: scrollable region", i);
        }
    });

    ImGui::SameLine();

    CHILD(Options, 0.3, 1, {
        ImGui::Text("Options");
        for (int i = 0; i < 100; i++) {
            ImGui::Text("%04d: scrollable region", i);
        }
    });

    ImGui::SameLine();

    if (ImGui::Begin(
            "Preview", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_AlwaysAutoResize)) {
        auto *central_node = ImGui::DockBuilderGetCentralNode(dockspace_id);

        ImGui::SetWindowPos(central_node->Pos);
        ImGui::SetWindowSize(central_node->Size);
        ImGui::Text("Preview");
        ImGui::End();
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
}

void DebugUI::reloadState() {}

void DebugUI::modalPopup(std::string /*message*/) {
    // TODO: Implement this
}

void DebugUI::errorPopup(std::string message) {
    errorMessages_.push_back(message);
}

void DebugUI::handleInput() {
    for (ImGuiKey key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END;
         key = (ImGuiKey)(key + 1)) {
        if (!ImGui::IsKeyPressed(key)) {
            continue;
        }

        std::string name{ImGui::GetKeyName(key)};
        if (name == "PageUp") {
            visible_ = !visible_;
        }
    }
}

void DebugUI::drawEntityBrowser() {
    //  ImVec2 proportions = ImGui::GetContentRegionAvail();
    if (ImGui::Begin("Entity Browser")) {
        ImGuiTreeNodeFlags flags{ImGuiTreeNodeFlags_DefaultOpen |
                                 ImGuiTreeNodeFlags_Leaf};

        std::vector<entity_ptr> entities{EntityManager::get().findAll()};

        for (const auto &entity_ptr : entities) {
            entity_id_t id{entity_ptr->getId()};
            {
                std::string label{
                    std::format("({}) {}", id, entity_ptr->getName())};

                if (ImGui::TreeNodeEx(label.data(),
                                      flags | (entity_ptr == selectedEntity_
                                                   ? ImGuiTreeNodeFlags_Selected
                                                   : ImGuiTreeNodeFlags_None),
                                      "%s", label.data())) {
                    if (ImGui::IsItemClicked()) {
                        selectedEntity_ = entity_ptr;
                    };

                    ImGui::TreePop();
                }
            };
        }

        ImGui::End();
    }
}

void DebugUI::drawComponentBrowser() {
    if (ImGui::Begin("Component Browser")) {
        ImGui::BeginTable(
            "ComponentBrowserTable", 2,
            ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersH);

        ImGui::TableNextColumn();

        ImGuiTreeNodeFlags flags{ImGuiTreeNodeFlags_DefaultOpen |
                                 ImGuiTreeNodeFlags_Leaf};

        auto bits{DebugUI::componentBits_};

        auto components{EntityManager::get().findComponentsWhere(
            [bits](const GenericComponentPtr &component) {
                if (bits == -1U) {
                    return true;
                }

                if (((bits & TRANSFORM_BIT) != 0U) &&
                    component->isOfType<TransformComponent>()) {
                    return true;
                }
                if (((bits & MODEL_BIT) != 0U) &&
                    component->isOfType<ModelComponent>()) {
                    return true;
                }

                return false;
            })};

        for (const GenericComponentPtr &component : components) {
            auto id{component->getId()};
            {
                std::string label{std::format("({}) Component", id)};

                if (ImGui::TreeNodeEx(
                        label.data(),
                        flags | (/*entity_ptr == selectedEntity_*/ true
                                     ? ImGuiTreeNodeFlags_Selected
                                     : ImGuiTreeNodeFlags_None),
                        "%s", label.data())) {
                    if (ImGui::IsItemClicked()) {
                        // selectedEntity_ = entity_ptr;
                    };

                    ImGui::TreePop();
                }
            };
        }

        ImGui::TableNextColumn();
        ImGui::Text("Filters");

        auto make_checkbox{[bits](const char *label, CheckedComponentBit bit) {
            bool enabled{static_cast<bool>(bits & bit)};
            if (ImGui::Checkbox(label, &enabled)) {
                DebugUI::componentBits_ = DebugUI::componentBits_ ^ bit;
            };
        }};

        bool all_enabled{DebugUI::componentBits_ == -1U};
        if (ImGui::Checkbox("All", &all_enabled)) {
            if (DebugUI::componentBits_ == 0) {
                DebugUI::componentBits_ = ~0U;
            } else if (DebugUI::componentBits_ != -1U) {
                DebugUI::componentBits_ = -1U;
            } else {
                DebugUI::componentBits_ = 0;
            }
        }

        make_checkbox("Transform", TRANSFORM_BIT);
        make_checkbox("Model", MODEL_BIT);
        make_checkbox("Camera", CAMERA_BIT);

        ImGui::EndTable();

        ImGui::End();
    }
}

}  // namespace Presto
