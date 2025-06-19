module presto.physics;

import presto.internal.managers;

void Presto::AddPersistentForce(Force force) {
    PhysicsManager::get().addPersistentForce(force);
};
