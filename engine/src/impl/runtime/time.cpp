#include "presto/runtime/time.h"

#include "presto/internal/pch.h"

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::nanoseconds;

using TimePoint = std::chrono::time_point<Clock, Duration>;

namespace Presto {
struct Time::Impl {
    TimePoint program_start;
    TimePoint current_time;
    Duration delta_time{0};
};

std::unique_ptr<Time::Impl> Time::impl_{std::make_unique<Time::Impl>()};

TimePoint getCurrentTime() { return std::chrono::high_resolution_clock::now(); }
void Time::init() {
    impl_->program_start = getCurrentTime();
    impl_->current_time = impl_->program_start;
}

void Time::update() {
    auto current_time = getCurrentTime();

    auto delta = current_time - impl_->current_time;

    impl_->delta_time = delta;
    impl_->current_time = current_time;
}

Time::Seconds Time::deltaSeconds() {
    return std::chrono::duration<double>(impl_->delta_time).count();
}

Time::Milliseconds Time::deltaMilliseconds() {
    return std::chrono::duration<double, std::milli>(impl_->delta_time).count();

    /*
return std::chrono::duration_cast<std::chrono::milliseconds>(
       impl_->delta_time)
.count();
            */
};

Time::Seconds Time::totalSecondsSinceStart() {
    return std::chrono::duration<double>(impl_->current_time -
                                         impl_->program_start)
        .count();
}

Timer::Timer(Time::Milliseconds duration) { setLength(duration); }

void Timer::start() {
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
