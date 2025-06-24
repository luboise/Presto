#pragma once

#include "presto/utils/file.h"           // IWYU pragma: export
#include "presto/utils/interpolation.h"  // IWYU pragma: export
#include "presto/utils/typemap.h"        // IWYU pragma: export

#include "presto/core/types.h"

#include "presto/runtime/time.h"

namespace Pr {

template <class T>
class Singleton {
   public:
    [[nodiscard]] static T& Get() {
        static T instance{};
        return instance;
    };
};

class LazyCalculator {
    using DirtyBitType = bool;

   public:
    [[nodiscard]] bool isDirty() const { return dirty_; }

   protected:
    void setDirty(DirtyBitType dirty = true) { dirty_ = dirty; }

   private:
    DirtyBitType dirty_;
};

class DebugTimer {
   public:
    explicit DebugTimer(Pr::string name);

    void printElapsed(bool resetTimer = false);
    void reset();

    ~DebugTimer() = default;

   private:
    Pr::string name;
    Time::Milliseconds time_started{0};
    Time::Milliseconds time_ended{0};
};

}  // namespace Pr
