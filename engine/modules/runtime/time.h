module;
#include "presto/platform.h"

export module presto.runtime.time;

import presto.core;
import presto.core.concepts;

export namespace Presto {

class PRESTO_API Time {
    friend class Application;

   public:
    using Seconds = double;
    using Milliseconds = double;

    static Milliseconds deltaMilliseconds();
    static Seconds deltaSeconds();

    static Seconds totalSecondsSinceStart();

   private:
    static void update();
    static void init();

    struct Impl;
    static Allocated<Impl> impl_;
};

class PRESTO_API Timer {
   public:
    Timer() = default;

    /** Creates a new timer.
     *@param duration The length of the timer in milliseconds
     */
    explicit Timer(Time::Milliseconds duration);

    void start();

    void reset(Time::Milliseconds duration);

    void setLength(Time::Milliseconds duration);

    [[nodiscard]] bool running() const { return started_; };
    [[nodiscard]] bool finished() const {
        return started_ && (progress() == 1);
    };

    [[nodiscard]] Scalar progress() const;

   private:
    bool started_{false};
    bool finished_{false};

    Time::Seconds duration_{0};
    Time::Seconds startTime_{0};
};

}  // namespace Presto
