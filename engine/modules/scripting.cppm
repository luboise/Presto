export module presto.scripting;

import std;

import presto.core;
import presto.core.eventmanager;
import presto.core.concepts;
import presto.objects;
import presto.objects.components;

// TODO: Check this
// #include "Presto/Debugging.h"  // IWYU pragma: export

export namespace Pr {

class EventListener {
   public:
    // Adds a new listener to the group
    template <typename E>
        requires DerivedFrom<E, Event>
    void on(EventHandler<E> handler) {
        EventManager::Get().addHandler(std::move(handler));
    };
};

#define FETCHED(TYPE, NAME)                                                    \
    Pr::Ptr<TYPE> NAME {                                                       \
        [this]() {                                                             \
            Pr::ConductorComponent::addPreStartCallback([this]() -> void {     \
                auto ptr{Conductor::entity->getComponent<TYPE>()};             \
                PR_ASSERT(ptr != nullptr, "Fetched pointer can not be null."); \
                this->NAME = ptr;                                              \
            });                                                                \
            return nullptr;                                                    \
        }()                                                                    \
    }

#define FETCHED_OR_NULL(TYPE, NAME)                                    \
    [this]() {                                                         \
        Pr::ConductorComponent::addPreStartCallback([this]() -> void { \
            this->NAME = ptr;                                          \
            return Conductor::entity->getComponent<TYPE>();            \
        });                                                            \
        return nullptr;                                                \
    }()

}  // namespace Pr
