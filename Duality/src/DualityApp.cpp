#include <Presto/Presto.h>

namespace Pr = Presto;

class DualityApp : public Presto::Application {
   public:
    DualityApp() { this->_heart = Pr::EntityManager::newEntity(); }

    ~DualityApp() {}

   private:
    Pr::Entity* _heart;
};

Presto::Application* Presto::CreateApplication() { return new DualityApp(); }