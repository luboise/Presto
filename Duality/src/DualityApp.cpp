#include <Presto.h>

class DualityApp : public Presto::Application {
   public:
    DualityApp(){}
    ~DualityApp(){}

    void Run() {
        for (auto i = 0; i < 10; i++) {
            Presto::Thumbstick vals = Presto::InputManager::GetLeftThumbStickXY();
            PR_INFO("Controller 1\nX: {}\nY: {}", vals.X, vals.Y);
        }
    }
};

Presto::Application* Presto::CreateApplication() { return new DualityApp(); }