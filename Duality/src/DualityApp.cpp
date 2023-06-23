#include <Presto.h>

class DualityApp : public Presto::Application {
   public:
    DualityApp(){}
    ~DualityApp(){}

    void Run() {
        Presto::InputManager::TogglePolling();
        while (true) {
        }
    }
};

Presto::Application* Presto::CreateApplication() { return new DualityApp(); }