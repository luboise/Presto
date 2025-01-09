#include "Presto/Presto.h"

#include "EditorUI.h"

PRESTO_ENTRY_POINT();

class PrestoEditor : public Presto::Application {
   public:
    void setup() override {
        EditorUI::initialise(this->GetWindow(), [this] { this->exit(); });
    }

    void preLoop() override { EditorUI::draw(); }

    void postLoop() override { EditorUI::render(); }

    void tearDown() override {}
};

Presto::Application* Presto::CreateApplication() { return new PrestoEditor(); };
