#include "Presto/Utils/Timer.h"
#include "Presto/Core/Concepts.h"

namespace Presto {
    Timer::Timer(Time::Milliseconds duration) { setLength(duration); }

    void Timer::start() {
        /*
if (this->running_) {
    return;
}
        */

        startTime_ = Time::totalSecondsSinceStart();
        started_ = true;
    };

    Scalar Timer::progress() const {
        // If the timer has finished, it's at 100% progress
        if (finished_) {
            return 1;
        }
        // If the timer hasn't finished, or started, then its at 0% progress
        if (!started_) {
            return 0;
        }

        // Otherwise, calculate the progress

        double currentTime{Time::totalSecondsSinceStart()};

        PR_ASSERT(duration_ != 0, "Timer has a duration of 0.");

        auto percentage{
            static_cast<Scalar>((currentTime - startTime_) / duration_)};

        return std::clamp(percentage, 0.0F, 1.0F);
    };

    void Timer::setLength(Time::Milliseconds duration) {
        duration_ = duration / 1000.0;
    };

    void Timer::reset(Time::Milliseconds duration) {
        started_ = false;
        this->setLength(duration);
    };
}  // namespace Presto
