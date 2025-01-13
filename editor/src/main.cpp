#include "Presto/Presto.h"

#include "EditorUI.h"
#include "MasterpieceManager.h"

PRESTO_ENTRY_POINT();

class PrestoEditor : public Presto::Application {
   public:
    void setup() override {
        EditorUI::initialise(this->GetWindow(), [this] { this->exit(); });
        MasterpieceManager::Init();
    }

    void GameLoop() override {
        auto time = Presto::Time::totalSecondsSinceStart();

        _mainCamera->setFocus({0, 50, 0});

        constexpr auto camera_distance = 150;
        constexpr auto camera_height = 165;

        _mainCamera->setPos({glm::sin(time) * camera_distance, camera_height,
                             glm::cos(time) * camera_distance});
        // _mainCamera.setYaw(diff);
    }

    void preLoop() override { EditorUI::draw(); }

    void postLoop() override { EditorUI::render(); }

    void tearDown() override {}
};

Presto::Application* Presto::CreateApplication() { return new PrestoEditor(); };
