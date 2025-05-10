#pragma once

#define FETCHED(TYPE, NAME)                                            \
    Presto::Ptr<TYPE> NAME {                                           \
        [this]() {                                                     \
            Presto::ConductorComponent::addPreStartCallback(           \
                [this]() -> Presto::Ptr<TYPE> {                        \
                    auto ptr{Conductor::entity->getComponent<TYPE>()}; \
                    PR_ASSERT(ptr != nullptr,                          \
                              "Fetched pointer can not be null.");     \
                    return ptr;                                        \
                });                                                    \
            return nullptr;                                            \
        }()                                                            \
    }

#define FETCHED_OR_NULL(TYPE, NAME)                             \
    [this]() {                                                  \
        Presto::ConductorComponent::addPreStartCallback(        \
            [this]() -> Presto::Ptr<TYPE> {                     \
                return Conductor::entity->getComponent<TYPE>(); \
            });                                                 \
        return nullptr;                                         \
    }()
