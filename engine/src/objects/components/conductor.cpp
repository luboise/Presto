#include "presto/objects/components/conductor.h"

void Pr::ConductorComponent::onEnterScene() {
    for (const auto& callback : preStartCallbacks_) {
        callback();
    }
    this->start();
}
void Pr::ConductorComponent::addPreStartCallback(
    const pre_start_callback_t& callback) {
    preStartCallbacks_.push_back(callback);
}
