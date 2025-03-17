#include "Presto/Physics.h"

#include "Modules/PhysicsManager.h"

void Presto::AddPersistentForce(Force force) {
    PhysicsManager::get().addPersistentForce(force);
};
