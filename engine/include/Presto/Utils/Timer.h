#pragma once

#include "Presto/Core/Concepts.h"

#include "Presto/Runtime/Time.h"

namespace Presto {
class Timer {
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
