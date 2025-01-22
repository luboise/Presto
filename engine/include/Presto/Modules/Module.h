#pragma once

// #include "Presto/Core.h"
#include <memory>

#include "Presto/Platform.h"

namespace Presto {
    template <typename T>
    class PRESTO_API Module {
        friend class Application;

       public:
        static T& get() {
            PR_CORE_ASSERT(
                T::initialised(),
                std::string("Attempted to get uninitialised module: ") +
                    typeid(T).name());

            return *instance_;
        }

        static bool initialised() { return instance_ != nullptr; }

        ~Module() {
            if (T::initialised()) {
                instance_.reset();
            }
        };

       protected:
        static std::unique_ptr<T> instance_;

       private:
        virtual void update() = 0;
        virtual void onInit() {};

        static void init() {
            static_assert(false, "All modules must override Init().");
        };

        static void shutdown() {
            static_assert(false, "All modules must override Shutdown().");
        };
    };

    template <typename T>
    std::unique_ptr<T> Module<T>::instance_;

}  // namespace Presto
