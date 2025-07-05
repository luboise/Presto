#pragma once
#include "presto/platform.h"  // IWYU pragma: export

#define MODULE_FUNCTIONS(Type)                                  \
                                                                \
   private:                                                     \
    friend class Module<Type>;                                  \
                                                                \
   public:                                                      \
    [[nodiscard]] static constexpr Pr::string getModuleName() { \
        return #Type;                                           \
    }                                                           \
                                                                \
    Type(const Type&) = delete;                                 \
    Type(Type&&) = delete;                                      \
    Type& operator=(const Type&) = delete;                      \
    Type& operator=(Type&&) = delete;                           \
                                                                \
   private:                                                     \
    friend class Application
