#include <algorithm>
#include "presto/core/platform.h"

#include "presto/core/concepts.h"
#include "presto/runtime.h"

namespace Pr {

template <Scalable T>
class PRESTO_API Interpolator {
   public:
    explicit Interpolator() = default;
    explicit Interpolator(T val) : Interpolator() { setValue(val); };

    void setValue(T newDiff) { value_ = newDiff; }

    T getRatio(Scalar ratio) {
        ratio = std::clamp(ratio, 0.0F, 1.0F);
        return value_ * ratio;
    };

   private:
    T value_;
};

template <Scalable T>
class PRESTO_API TimedInterpolator : private Interpolator<T> {
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

}  // namespace Pr
