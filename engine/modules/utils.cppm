export module presto.utils;

export import :file;
export import :interpolation;
export import :typemap;

import presto.types.core;

import presto.runtime.time;

export namespace Pr {

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
