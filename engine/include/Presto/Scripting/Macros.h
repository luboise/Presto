#pragma once

#define FETCHED(TYPE, NAME)                                                    \
    Presto::Ptr<TYPE> NAME {                                                   \
        [this]() {                                                             \
            Presto::ConductorComponent::addPreStartCallback([this]() -> void { \
                auto ptr{Conductor::entity->getComponent<TYPE>()};             \
                PR_ASSERT(ptr != nullptr, "Fetched pointer can not be null."); \
                this->NAME = ptr;                                              \
            });                                                                \
            return nullptr;                                                    \
        }()                                                                    \
    }

#define FETCHED_OR_NULL(TYPE, NAME)                                        \
    [this]() {                                                             \
        Presto::ConductorComponent::addPreStartCallback([this]() -> void { \
            this->NAME = ptr;                                              \
            return Conductor::entity->getComponent<TYPE>();                \
        });                                                                \
        return nullptr;                                                    \
    }()
