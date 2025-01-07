#include "Presto/Runtime/Time.h"

#include <chrono>
#include <memory>

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

    TimePoint getCurrentTime() {
        return std::chrono::high_resolution_clock::now();
    }
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

    double Time::deltaSeconds() { return Time::deltaMilliseconds() / 1000; }

    double Time::deltaMilliseconds() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   impl_->delta_time)
            .count();
    };

    double Time::totalSecondsSinceStart() {
        return std::chrono::duration_cast<std::chrono::seconds>(
                   impl_->current_time - impl_->program_start)
            .count();
    }
}  // namespace Presto
