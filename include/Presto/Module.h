#pragma once

#include "Presto/Core.h"
#define F_INIT Init
#define F_UPDATE Update
#define F_SHUTDOWN Shutdown

/*
#define MODULE_FUNCTIONS()                   \
   public:                                   \
    static void F_UPDATE();                  \
    static void F_SHUTDOWN();                \
                                             \
   private:                                  \
    inline static bool _initialised = false; \
    static bool IsInitialised() { return _initialised; };
        */

namespace Presto {
    template <typename T>
    class PRESTO_API Module {
       public:
        static void Init() {
            static_assert(false, "All modules must override Init().");
        };
        virtual void Update() = 0;
        static void Shutdown() {
            static_assert(false, "All modules must override Shutdown().");
        };

        virtual void OnInit() {};

        static T& Get() {
            PR_CORE_ASSERT(_instance != nullptr,
                           "Attempted to get uninitialised module: {}",
                           typeid(T).name());

            return *_instance;
        }

        static bool IsInitialised() { return _instance != nullptr; }

        ~Module() = default;

       protected:
        static T* _instance;
    };

    template <typename T>
    T* Module<T>::_instance = nullptr;
}  // namespace Presto
