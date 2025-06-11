#pragma once

namespace Presto {

template <class T>
class Singleton {
   public:
    [[nodiscard]] static T& Get() {
        static T instance{};
        return instance;
    };
};

}  // namespace Presto
