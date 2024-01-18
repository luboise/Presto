#include <Presto.h>

// #include <Presto/EntryPoint.h>

class DualityApp : public Presto::Application {
   public:
    DualityApp() {}
    ~DualityApp() {}
};

Presto::Application* Presto::CreateApplication() { return new DualityApp(); }