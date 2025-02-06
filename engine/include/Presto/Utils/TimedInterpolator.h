#pragma once

#include "Presto/Core/Concepts.h"

#include "Presto/Runtime/Time.h"

#include "Presto/Utils/Interpolator.h"
#include "Presto/Utils/Timer.h"

namespace Presto {
template <Scalable T>
class TimedInterpolator : private Interpolator<T> {
   public:
    TimedInterpolator() = default;

    TimedInterpolator(T value, Time::Milliseconds length)
        : TimedInterpolator() {
        this->reset(value, length);
    }

    [[nodiscard]] bool completed() const { return timer_.finished(); };
    [[nodiscard]] bool inProgress() const { return timer_.running(); };

    T value() { return Interpolator<T>::getRatio(timer_.progress()); };

    /** @brief Resets the timed interpolator and starts calculating
     * immediately.
     * @param value The value to interpolate to
     * @param duration The new length of the timer in milliseconds
     */
    void reset(T value, Time::Milliseconds duration) {
        this->setValue(value);
        timer_.reset(duration);
        timer_.start();
    };

   private:
    Timer timer_;
};

}  // namespace Presto
