#include <Presto/Presto.h>
#include <iostream>

class PrestoEditor : public Presto::Application {
    void Setup() override {
        auto entity = Presto::EntityManager::newEntity();
        Presto::EntityManager::destroyEntity(entity);

        this->editingMode = true;
    }
};
