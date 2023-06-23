#include <Presto.h>

#include <thread>
#include <chrono>
using namespace std::chrono_literals;

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