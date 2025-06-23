#include "presto/internal/utils.h"

#include <utility>

#include "presto/core.h"

DebugTimer::DebugTimer(Presto::string name)
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
