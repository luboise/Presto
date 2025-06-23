#include "presto/internal/debugging.h"
#include "presto/internal.h"

#include "presto/aliases/object.h"
#include "presto/internal/managers.h"
#include "presto/objects.h"
#include "presto/runtime/window.h"

#include <algorithm>
#include <memory>
#include <utility>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace Presto {

void DebugUI::initialise(Presto::Window* windowPtr,
                         std::function<void()> exitCallback) {
    DebugUI::exitCallback_ = std::move(exitCallback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    auto* window{static_cast<GLFWwindow*>(windowPtr->getWindowPtr())};

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init();

    debugCamera_ = DebugCameraListener(RenderingManager::get());
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
            body                                                 \
                                                                 \
            ImGui::End();                                        \
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

    /*
if (showEntityBrowser_) {
    drawEntityBrowser();
};
    */

    if (showComponentBrowser_) {
        drawComponentBrowser();
    };

    if (showCameraBrowser_) {
        drawCameraBrowser();
    }

    /*
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
            */

    if (ImGui::Begin(
            "Preview", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_AlwaysAutoResize)) {
        auto* central_node = ImGui::DockBuilderGetCentralNode(dockspace_id);

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
    static ImGuiIO& io = ImGui::GetIO();
    static float movement_speed{5};

    if (debugCamera_.enabled()) {
        Camera& camera{debugCamera_.camera()};

        if (ImGui::IsKeyPressed(ImGuiKey_7)) {
            movement_speed *= 2;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_6)) {
            movement_speed = ImClamp(movement_speed / 2.0F, 0.3F, 9000.0F);
        }

        float delta_scale{movement_speed * io.DeltaTime};

        if (ImGui::IsKeyDown(ImGuiKey_W)) {
            vec3 fwrds{camera.transformData().forwards()};
            camera.setPosition(camera.position() + delta_scale * fwrds);
        }
        if (ImGui::IsKeyDown(ImGuiKey_A)) {
            vec3 lftwards{camera.transformData().leftwards()};
            camera.setPosition(camera.position() + delta_scale * lftwards);
        }
        if (ImGui::IsKeyDown(ImGuiKey_S)) {
            vec3 fwrds{camera.transformData().forwards()};
            camera.setPosition(camera.position() - delta_scale * fwrds);
        }
        if (ImGui::IsKeyDown(ImGuiKey_D)) {
            vec3 lftwards{camera.transformData().leftwards()};
            camera.setPosition(camera.position() - delta_scale * lftwards);
        }

        camera.rotate(0.5F * vec3{-io.MouseDelta.y, -io.MouseDelta.x, 0});

        // auto new_rot{camera.rotation().toEuler() +
        // 0.5F * vec3{-io.MouseDelta.y, -io.MouseDelta.x, 0}};

        /*
if (new_rot.x >= 90) {
    new_rot.x = 89.5;
} else if (new_rot.x <= -90) {
    new_rot.x = -89.5;
}
        */

        // camera.setRotation(new_rot);
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Home, false)) {
        visible_ = !visible_;
        DebugManager::get().setDrawMainCamera(visible_);
    }

    if (ImGui::IsKeyPressed(ImGuiKey_End, false)) {
        debugCamera_.toggle();
    }
}

void DebugUI::drawEntityBrowser() {
    //  ImVec2 proportions = ImGui::GetContentRegionAvail();
    if (ImGui::Begin("Entity Browser")) {
        ImGuiTreeNodeFlags flags{ImGuiTreeNodeFlags_DefaultOpen |
                                 ImGuiTreeNodeFlags_Leaf};

        std::vector<EntityPtr> entities{EntityManagerImpl::get().findAll()};

        for (const auto& entity_ptr : entities) {
            entity_id_t id{entity_ptr->id()};
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
            "ComponentBrowserTable", 3,
            ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersH);

        ImGui::TableNextColumn();

        ImGuiTreeNodeFlags flags{ImGuiTreeNodeFlags_DefaultOpen |
                                 ImGuiTreeNodeFlags_Leaf};

        auto bits{DebugUI::componentBits_};

        auto components_view{EntityManager::Get().findComponentsWhere(
            [bits](const GenericComponentPtr& component) {
                if (bits == -1U) {
                    return true;
                }

                if (((bits & TRANSFORM_BIT) != 0U) &&
                    component->isOfType<TransformComponent>()) {
                    return true;
                }
                if (((bits & RENDER_BIT) != 0U) &&
                    component->isOfType<RenderComponent>()) {
                    return true;
                }
                if (((bits & CAMERA_BIT) != 0U) &&
                    component->isOfType<CameraComponent>()) {
                    return true;
                }

                return false;
            })};

        std::vector<Ptr<Component>> components(components_view.begin(),
                                               components_view.end());

        std::ranges::sort(
            components,
            [](const Ptr<Component>& a, const Ptr<Component>& b) -> bool {
                return a->id() < b->id();
            });

        for (const GenericComponentPtr& component : components) {
            auto id{component->id()};
            {
                std::string label{std::format("({}) Component", id)};

                if (ImGui::TreeNodeEx(
                        label.data(),
                        flags | (/*entity_ptr == selectedEntity_*/ true
                                     ? ImGuiTreeNodeFlags_Selected
                                     : ImGuiTreeNodeFlags_None),
                        "%s", label.data())) {
                    if (ImGui::IsItemClicked()) {
                        selectedComponent_ = component;
                    };

                    ImGui::TreePop();
                }
            };
        }

        ImGui::TableNextColumn();
        ImGui::Text("Filters");

        auto make_checkbox{[bits](const char* label, CheckedComponentBit bit) {
            bool enabled{static_cast<bool>(bits & bit)};
            if (ImGui::Checkbox(label, &enabled)) {
                DebugUI::componentBits_ ^= bit;
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
        make_checkbox("Model", RENDER_BIT);
        make_checkbox("Camera", CAMERA_BIT);

        if (selectedComponent_ != nullptr) {
            ImGui::TableNextColumn();
            drawSelectedComponent();
        }

        ImGui::EndTable();

        ImGui::End();
    }
}

void DebugUI::drawSelectedComponent() {
    ImGui::Text("Component Editor");

    PR_CORE_ASSERT(selectedComponent_ != nullptr,
                   "drawSelectedComponent shouldn't be called if the selected "
                   "component is null.");

    if (selectedComponent_->isOfType<CameraComponent>()) {
        drawCameraModifier(
            std::dynamic_pointer_cast<Camera>(selectedComponent_));
    }
};

void DebugUI::drawCameraModifier(Ptr<Camera> cameraPtr) {
    if (cameraPtr == nullptr) {
        ImGui::Text("No camera present.");
        return;
    }
    drawCameraModifier(*cameraPtr);
}

void DebugUI::drawCameraModifier(CameraComponent& camera) {
    DebugComponents::EnumChooser(
        camera.type(),
        std::vector<EnumMember<CameraType>>{
            {.value = CameraType::PERSPECTIVE, .label = "Perspective"},
            {.value = CameraType::ORTHOGRAPHIC, .label = "Orthographic"}});

    auto& extents{camera.extents()};

    DebugComponents::SliderChooser(
        extents.width, "Extents width", 1, 3840,
        [&camera, &extents](auto value) {
            camera.setExtents({.width = value, .height = extents.height});
        });

    DebugComponents::SliderChooser(
        extents.height, "Extents height", 1, 2160,
        [&camera, &extents](auto value) {
            camera.setExtents({.width = extents.width, .height = value});
        });

    auto distances{camera.distances()};

    Presto::vec3 pos{camera.position()};
    DebugComponents::Vec3Chooser(
        pos, "Position",
        [&camera](Presto::vec3 newPos) { camera.setPosition(newPos); });

    Presto::vec3 rot{camera.rotation().toEuler()};
    DebugComponents::Vec3Chooser(rot, "Rotation", [&camera](Presto::vec3 rot) {
        camera.setRotation(rot);
    });

    DebugComponents::SliderChooser(
        distances.near, "Near", -1000.F, 3000.F,
        [&camera, &distances](auto value) {
            if (value > distances.far) {
                return;
            }

            camera.setDistances({value, distances.far});
        });

    DebugComponents::SliderChooser(
        distances.far, "Far", -1000.F, 3000.F,
        [&camera, &distances](auto value) {
            if (value < distances.near) {
                return;
            }
            camera.setDistances({distances.near, value});
        });

    auto fov{static_cast<float>(glm::degrees(camera.FOV()))};
    DebugComponents::SliderChooser(
        fov, "FOV", 1.0F, 179.0F,
        [&camera](double value) { camera.setFOV(value); });
};

void DebugUI::drawCameraBrowser() {
    static auto& rm{RenderingManager::get()};

    if (ImGui::Begin("Camera Browser")) {
        Ptr<Camera> main_camera{rm.getMainCamera()->getComponent<Camera>()};

        static Ptr<Camera> current_camera{main_camera};

        auto enabled{debugCamera_.enabled()};
        DebugComponents::CheckboxChooser(
            enabled, "Use Debug Camera",
            [](bool /*unused*/) { debugCamera_.toggle(); });

        ComponentSearchResults components{
            EntityManager::Get().findComponentsWhere(
                [](const GenericComponentPtr& component) {
                    return component->template isOfType<Camera>();
                })};

        std::vector<Ptr<Camera>> cameras;
        std::ranges::for_each(components, [&cameras](auto& component) {
            cameras.push_back(std::dynamic_pointer_cast<Camera>(component));
        });

        if (ImGui::BeginTable("Camera Browser Internals", 2,
                              ImGuiTableFlags_BordersV)) {
            ImGui::TableNextColumn();

            ImGui::BeginDisabled(debugCamera_.enabled());
            if (ImGui::BeginTable("Camera Browser", 4,
                                  ImGuiTableFlags_SizingStretchProp |
                                      ImGuiTableFlags_BordersH)) {
                ImGui::TableSetupColumn("Main");
                ImGui::TableSetupColumn("ID");
                ImGui::TableSetupColumn("Type");
                ImGui::TableSetupColumn("###Edit Button");

                ImGui::TableHeadersRow();

                std::ranges::for_each(cameras, [&main_camera](
                                                   Ptr<Camera>& camera) {
                    ImGui::TableNextRow();

                    bool camera_is_main{camera == main_camera};

                    ImGui::PushID(static_cast<int>(camera->id()));

                    ImGui::TableNextColumn();

                    if (ImGui::Checkbox("", &camera_is_main)) {
                    }

                    ImGui::TableNextColumn();

                    ImGui::Text("%u", camera->id());

                    ImGui::TableNextColumn();
                    ImGui::Text("%s", camera->type() == CameraType::PERSPECTIVE
                                          ? "Perspective"
                                          : "Orthographic");

                    ImGui::TableNextColumn();

                    if (ImGui::Button("Edit")) {
                        current_camera = camera;
                    }

                    ImGui::PopID();
                });

                ImGui::EndTable();
            }
            ImGui::EndDisabled();

            ImGui::TableNextColumn();

            if (debugCamera_.enabled()) {
                drawCameraModifier(debugCamera_.camera());
            } else {
                drawCameraModifier(current_camera);
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }
};

DebugCameraListener::DebugCameraListener() { camera_ = nullptr; };

DebugCameraListener::DebugCameraListener(RenderingManager& rm) {
    camera_ = rm.getDebugCamera();
};

void DebugCameraListener::toggle() {
    if (camera_ == nullptr) {
        PR_CORE_ERROR("Unable to doggle debug camera on, as it is null.");
        return;
    }

    enabled_ = !enabled_;
    RenderingManager::get().setUsingDebugCamera(enabled_);
};

bool DebugCameraListener::enabled() const { return enabled_; };

CameraComponent& DebugCameraListener::camera() { return *camera_; };
}  // namespace Presto
