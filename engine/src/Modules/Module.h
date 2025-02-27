#pragma once

// #include "Presto/Core.h"
#include <format>

#include <functional>
#include <memory>
#include <type_traits>

#include "Presto/Core/Assert.h"
#include "Presto/Platform.h"

#define MODULE_FUNCTIONS(Type)                                    \
                                                                  \
   private:                                                       \
    friend class Module<Type>;                                    \
                                                                  \
   public:                                                        \
    [[nodiscard]] static constexpr PR_STRING_ID getModuleName() { \
        return #Type;                                             \
    }                                                             \
                                                                  \
    Type(const Type&) = delete;                                   \
    Type(Type&&) = delete;                                        \
    Type& operator=(const Type&) = delete;                        \
    Type& operator=(Type&&) = delete;                             \
                                                                  \
   private:                                                       \
    friend class Application

#define INTERNAL_MODULE_STATIC_ASSERTION()                               \
    static_assert(UsesModuleFunctions<T>::value,                         \
                  "The type T must use the MODULE_FUNCTIONS() macro to " \
                  "define a static function `getModuleName()`.")

namespace Presto {
template <typename T, typename = void>
struct UsesModuleFunctions : std::false_type {};

template <typename T>
struct UsesModuleFunctions<T, std::void_t<decltype(&T::getModuleName)>>
    : std::true_type {};

template <class T>
// requires UsesModuleFunctions<T>::value
class PRESTO_API Module {
   public:
    friend class Application;

    /**
     * Returns a reference to the current instance of a given module.
     *
     * Examples:
     * ImportedMesh meshData{...}; // Existing imported mesh data
     *
     * RenderingManager::get().loadMesh(meshData);
     *
     * const auto& rm{RenderingManager::get()}; Valid usage.
     * auto& rm{RenderingManager::get()}; Valid usage.
     * auto rm{RenderingManager::get()}; INVALID usage.
     *
     * rm.loadMesh(meshData);
     *
     * @brief Gets a reference to the current instance of a given manager.
     * @warning The base Module class has its copy and move constructors
     * deleted, meaning that only references can be retrieved from get().
     */
    static T& get() {
        INTERNAL_MODULE_STATIC_ASSERTION();
        PR_CORE_ASSERT(T::initialised(),
                       std::format("Attempted to get uninitialised module: {}",
                                   T::getModuleName()));

        return *instance_;
    }

    static bool initialised() { return instance_ != nullptr; }

    Module(const Module&) = delete;
    Module(Module&&) = delete;
    Module& operator=(const Module&) = delete;
    Module& operator=(Module&&) = delete;

   protected:
    using ModulePointer = std::unique_ptr<T, std::function<void(T*)>>;

    static ModulePointer instance_;
    Module() = default;
    virtual ~Module() = default;

   private:
    virtual void update() = 0;
    virtual void onInit() {};

    template <typename... Args>
    static void init(Args&&... args) {
        INTERNAL_MODULE_STATIC_ASSERTION();

        PR_ASSERT("Module {} must only be initialised once.",
                  T::getModuleName());

        PR_CORE_INFO("Initialising {}.", T::getModuleName());

        // Necessary, since the constructor is private
        instance_ = ModulePointer{new T(std::forward<Args>(args)...),
                                  [](T* ptr) { delete ptr; }};
    };

    static void shutdown() {
        INTERNAL_MODULE_STATIC_ASSERTION();

        PR_ASSERT("Module {} must be initialised in order to be destructed.",
                  typeid(T).name());
        instance_.reset();
    };
};

template <typename T>
Module<T>::ModulePointer Module<T>::instance_{nullptr};

}  // namespace Presto
