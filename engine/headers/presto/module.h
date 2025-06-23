#pragma once
#include "presto/platform.h"  // IWYU pragma: export

#define MODULE_FUNCTIONS(Type)                                      \
                                                                    \
   private:                                                         \
    friend class Module<Type>;                                      \
                                                                    \
   public:                                                          \
    [[nodiscard]] static constexpr Presto::string getModuleName() { \
        return #Type;                                               \
    }                                                               \
                                                                    \
    Type(const Type&) = delete;                                     \
    Type(Type&&) = delete;                                          \
    Type& operator=(const Type&) = delete;                          \
    Type& operator=(Type&&) = delete;                               \
                                                                    \
   private:                                                         \
    friend class Application

#define INTERNAL_MODULE_STATIC_ASSERTION()                               \
    static_assert(UsesModuleFunctions<T>::value,                         \
                  "The type T must use the MODULE_FUNCTIONS() macro to " \
                  "define a static function `getModuleName()`.")
