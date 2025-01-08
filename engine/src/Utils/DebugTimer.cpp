#include "DebugTimer.h"

#include <utility>

namespace Presto {
    DebugTimer::DebugTimer(std::string name)
        : name(std::move(name)), time_started(Time::totalSecondsSinceStart()) {}

    void DebugTimer::printElapsed(bool resetTimer) {
        time_ended = Time::totalSecondsSinceStart();
        PR_CORE_TRACE("Time Elapsed for timer {}: {}", name,
                      time_ended - time_started);

        if (resetTimer) {
            reset();
        }
    }

    void DebugTimer::reset() { time_started = time_ended; }
}  // namespace Presto
