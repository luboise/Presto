module presto.utils;

import std;
import presto.core.logging;

namespace Pr {

DebugTimer::DebugTimer(Pr::string name)
    : name(std::move(name)), time_started(Time::totalSecondsSinceStart()) {}

void DebugTimer::printElapsed(bool resetTimer) {
    time_ended = Time::totalSecondsSinceStart();
    Pr::CoreLog(TRACE, "Time Elapsed for timer {}: {}", name,
                time_ended - time_started);

    if (resetTimer) {
        reset();
    }
}

void DebugTimer::reset() { time_started = time_ended; }

}  // namespace Pr
