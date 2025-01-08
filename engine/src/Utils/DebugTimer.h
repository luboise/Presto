#include "Presto/Runtime/Time.h"

namespace Presto {
    class DebugTimer {
       public:
        explicit DebugTimer(std::string name);

        void printElapsed(bool resetTimer = false);
        void reset();

        ~DebugTimer() = default;

       private:
        std::string name;
        Time::Milliseconds time_started{0};
        Time::Milliseconds time_ended{0};
    };

}  // namespace Presto
