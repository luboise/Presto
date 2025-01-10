#pragma once

// #include "Presto/Core.h"
#include <memory>

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
        friend class Application;

       public:
        static T& Get() {
            PR_CORE_ASSERT(
                T::IsInitialised(),
                std::string("Attempted to get uninitialised module: ") +
                    typeid(T).name());

            return *instance_;
        }

        static bool IsInitialised() { return instance_ != nullptr; }

        ~Module() = default;

       protected:
        static std::unique_ptr<T> instance_;

       private:
        static void Init() {
            static_assert(false, "All modules must override Init().");
        };

        virtual void Update() = 0;
        static void Shutdown() {
            static_assert(false, "All modules must override Shutdown().");
        };

        virtual void OnInit() {};
    };

    template <typename T>
    std::unique_ptr<T> Module<T>::instance_ = nullptr;

}  // namespace Presto
