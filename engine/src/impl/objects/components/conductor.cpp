module presto.objects.components.conductor;

void Presto::ConductorComponent::onEnterScene() {
    for (const auto& callback : preStartCallbacks_) {
        callback();
    }
    this->start();
}
void Presto::ConductorComponent::addPreStartCallback(
    const pre_start_callback_t& callback) {
    preStartCallbacks_.push_back(callback);
}
